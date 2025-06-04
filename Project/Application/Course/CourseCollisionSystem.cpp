#include "CourseCollisionSystem.h"
#include <algorithm>
#include "../../Engine/base/BufferResource.h"
#include "../../Engine/base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "CoursePolygonType.h"
#include "../../Engine/2D/ImguiManager.h"
#include "../../Engine/base/Texture/TextureManager.h"
#include "Demo/CourseDemoObject.h"

#include "../Object/Player/Player.h"
#include "../Object/Car/CarLists.h"
#include "../Object/Player/State/User/PlayerInVehicleState.h"
#include <typeinfo>

// ポリゴンエリアの原点
const Vector3 CourseCollisionSystem::kPolygonAreasOrigin_ = { -500.0f, -500.0f, -500.0f };
// ポリゴンエリアの長さ
const Vector3 CourseCollisionSystem::kPolygonAreasLength_ = { 1000.0f, 1000.0f, 1000.0f };

// 衝突するオブジェクトキーワード
const std::array<std::string, CourseCollisionSystem::kCollidingObjectKeywordsMax_> 
CourseCollisionSystem::kCollidingObjectKeywords_ = 
{
	"CourseDemo", // コースデモオブジェクト
	"Player", // プレイヤー
	"Core", // ベーシックコア
	"Tire", // タイヤ
	"ArmorFrame", // フレーム
	"Engine" // エンジン
};

void CourseCollisionSystem::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();

	// パイプライン
	courseCollisionPipeline_ = std::make_unique<CourseCollisionPipeline>();
	courseCollisionPipeline_->Initialize(dxCommon_->GetDevice());

	// バッファ初期化
	BuffersInitialize();

	// コア
	vehicleCore_ = nullptr;

}

void CourseCollisionSystem::Execute()
{
	
	// 回数を初期化
	collisionCheakNum_ = 0;

	// 登録分回す
	for (std::list<CollisionObject>::iterator itr = collidingObjects_.begin();
		itr != collidingObjects_.end();++itr) {
		// CPU側でOBBとの距離判定をとる->メッシュ登録
		DistanceJudgment(*itr);

		// ->GPU側で押し出し距離確認
		ExtrusionExecuteCS();

		// 回数を増やす
		collisionCheakNum_++;

	}

	// キック
	CommadKick();

	// 回数を初期化
	collisionCheakNum_ = 0;

	// 登録分回す
	for (std::list<CollisionObject>::iterator itr = collidingObjects_.begin();
		itr != collidingObjects_.end(); ++itr) {

		// カートに属しているか
		bool belongToCart = 
			std::visit([&](auto x) {
			// 型
			using T = std::decay_t<decltype(x)>;
			// プレイヤー
			if constexpr (std::is_same_v<T, Player*>) {
				return false;
			}
			// デモオブジェクト
			else if constexpr (std::is_same_v<T, CourseDemoObject*>) {
				return false;
			}
			// プレイヤーじゃない
			else {
				CollisionCarObject collisionCarObject = x;
				// コアか
				if (std::holds_alternative<VehicleCore*>(collisionCarObject)) {
					// コアに代入
					vehicleCore_ = std::get<VehicleCore*>(x);
					return true;
				}
				bool belongToCartXX = 
					std::visit([&](auto xx) {
					return xx->IsParent();
					}, collisionCarObject);
				return belongToCartXX;
			}
			}, *itr);
		

		// カートに属しているか
		if (belongToCart) {
			// 登録
			belongsToCartPartsNumbers_.push_back(collisionCheakNum_);
		}
		// 個人勢
		else {
			// ->CPU側で押し出し、回転（壁データはとらない）
			// ->OBB登録のオブジェクトのワールドトランスフォーム更新
			AloneExtrusionCalculation(*itr);
		}
		// 回数を増やす
		collisionCheakNum_++;

	}

	// カートに属している勢
	CartExtrusionCalculation();

	// 後処理
	// 登録したオブジェクトリストをクリア
	collidingObjects_.clear();
	// カートに属しているパーツ&&衝突しているパーツの番号保存
	belongsToCartPartsNumbers_.clear();
	// コアをヌルに
	vehicleCore_ = nullptr;

}

void CourseCollisionSystem::ObjectRegistration(CollisionObject object)
{

	// オブジェクト登録数制限
	if (collidingObjects_.size() >= kObjectsThatCanBeRegisteredMax_) {
		return;
	}

	// オブジェクトリストに登録
	collidingObjects_.push_back(object);

}

void CourseCollisionSystem::ObjectRegistration(BaseObjectManager* objectManager)
{

	// オブジェクト取得
	std::list<BaseObjectManager::ObjectPair>* objects = objectManager->GetObjects();

	// オブジェクト
	for (std::list<BaseObjectManager::ObjectPair>::iterator itr = objects->begin();
		itr != objects->end(); ++itr) {

		// キーワード検索
		for (uint32_t i = 0; i < kCollidingObjectKeywordsMax_; ++i) {
			// キーワードに引っかかるか
			if (itr->first.find(kCollidingObjectKeywords_[i]) != std::string::npos) {
				// オブジェクトリストに登録
				switch (i)
				{
				case 0:
					collidingObjects_.push_back(static_cast<CourseDemoObject*>(itr->second.get()));
					break;
				case 1:
					ObjectRegistrationPlayer(static_cast<Player*>(itr->second.get()));
					break;
				case 2:
					collidingObjects_.push_back(static_cast<VehicleCore*>(itr->second.get()));
					break;
				case 3:
					collidingObjects_.push_back(static_cast<TireParts*>(itr->second.get()));
					break;
				case 4:
					collidingObjects_.push_back(static_cast<ArmorFrameParts*>(itr->second.get()));
					break;
				case 5:
					collidingObjects_.push_back(static_cast<EngineParts*>(itr->second.get()));
					break;
				default:
					break;
				}

				// キーワード検索から抜ける
				break;

			}

		}

		// オブジェクト登録数制限
		if (collidingObjects_.size() >= kObjectsThatCanBeRegisteredMax_) {
			break;
		}

	}

}

void CourseCollisionSystem::SetCourse(Course* course)
{

	// コース
	course_ = course;
	assert(course_);

	// ポリゴンエリアに登録
	int32_t x = 0, y = 0, z = 0;

	// 割る用の値
	Vector3 dividingValue = Vector3::Multiply(kPolygonAreasLength_, 1.0f / static_cast<float>(kPolygonAreasDiv_));

	// コース中心のワールド座標
	Vector3 worldPosition = course_->GetWorldTransformAdress()->GetWorldPosition();

	// コースメッシュ分回す
	std::vector<CoursePolygon>* polygons = course_->GetCoursePolygonsAdress();
	for (uint32_t i = 0; i < polygons->size(); ++i) {

		// ポリゴン
		CoursePolygon polygon = (*polygons)[i];

		// 頂点位置、原点調整
		Vector3 vertex0 = polygon.position0 - kPolygonAreasOrigin_ + worldPosition;
		Vector3 vertex1 = polygon.position1 - kPolygonAreasOrigin_ + worldPosition;
		Vector3 vertex2 = polygon.position2 - kPolygonAreasOrigin_ + worldPosition;

		// 重心
		Vector3 centerOfGravity = (vertex0 + vertex1 + vertex2) * (1.0f / 3.0f);

		// エリア番号
		x = static_cast<uint32_t>(centerOfGravity.x / dividingValue.x);
		y = static_cast<uint32_t>(centerOfGravity.y / dividingValue.y);
		z = static_cast<uint32_t>(centerOfGravity.z / dividingValue.z);

		// 登録（エリアをまたぐ場合、それぞれ登録）

		// 0番目
		polygonAreas[x][y][z].push_back(polygon);

	}

}

void CourseCollisionSystem::ImGuiDraw()
{

	ImGui::Begin("CourseCollisionSystem");
	// エリア表示モード番号
	std::string radioButtonName = "name";
	for (int32_t i = 0; i < kPolygonAreasDiv_; ++i) {
		radioButtonName = "areaDisplayX_" + std::to_string(i);
		ImGui::RadioButton(radioButtonName.c_str(), &areaDisplayX_, i); 
		if (i != kPolygonAreasDiv_ - 1) {
			ImGui::SameLine();
		}
	}
	for (int32_t i = 0; i < kPolygonAreasDiv_; ++i) {
		radioButtonName = "areaDisplayY_" + std::to_string(i);
		ImGui::RadioButton(radioButtonName.c_str(), &areaDisplayY_, i);
		if (i != kPolygonAreasDiv_ - 1) {
			ImGui::SameLine();
		}
	}
	for (int32_t i = 0; i < kPolygonAreasDiv_; ++i) {
		radioButtonName = "areaDisplayZ_" + std::to_string(i);
		ImGui::RadioButton(radioButtonName.c_str(), &areaDisplayZ_, i);
		if (i != kPolygonAreasDiv_ - 1) {
			ImGui::SameLine();
		}
	}

	ImGui::Text("X:%d Y:%d Z:%d", areaDisplayX_, areaDisplayY_, areaDisplayZ_);
	ImGui::Separator();

	CoursePolygon polygon = {};
	for (uint32_t i = 0; i < polygonAreas[areaDisplayX_][areaDisplayY_][areaDisplayZ_].size(); ++i) {

		polygon = polygonAreas[areaDisplayX_][areaDisplayY_][areaDisplayZ_][i];
		ImGui::Text("%d個目", i);
		ImGui::Text("位置0 x:%7.2f y:%7.2f z:%7.2f", polygon.position0.x, polygon.position0.y, polygon.position0.z);
		ImGui::Text("位置1 x:%7.2f y:%7.2f z:%7.2f", polygon.position1.x, polygon.position1.y, polygon.position1.z);
		ImGui::Text("位置2 x:%7.2f y:%7.2f z:%7.2f", polygon.position2.x, polygon.position2.y, polygon.position2.z);
		ImGui::Text("法線 x:%7.2f y:%7.2f z:%7.2f", polygon.normal.x, polygon.normal.y, polygon.normal.z);
		ImGui::Separator();

	}

	ImGui::End();

	// CS
	ImGui::Begin("CourseCollisionSystem_CS");

	// 登録分回す
	uint32_t count = 0;
	for (uint32_t i = 0; i < 1; ++i) {

		// オブジェクトデータ
		ImGui::Text("%d個目", count);
		ImGui::Text("center x:%7.2f y:%7.2f z:%7.2f", buffers_[count].objectMap_->center.x, buffers_[count].objectMap_->center.y, buffers_[count].objectMap_->center.z);

		// 出力データ
		ImGui::Text("center x:%7.2f y:%7.2f z:%7.2f", buffers_[count].outputDataMap_[0].extrusion.x, buffers_[count].outputDataMap_[0].extrusion.y, buffers_[count].outputDataMap_[0].extrusion.z);
		ImGui::Text("collided %d", buffers_[count].outputDataMap_[0].collided);
		ImGui::Text("drivingLocation %d", buffers_[count].outputDataMap_[0].drivingLocation);

		ImGui::Separator();
		count++;

	}

	ImGui::End();

}

void CourseCollisionSystem::BuffersInitialize()
{

	// デバイス
	ID3D12Device* device = DXGIDevice::GetInstance()->GetDevice();

	// オブジェクト分のバッファ
	for (uint32_t i = 0; i < kObjectsThatCanBeRegisteredMax_; ++i) {

		// オブジェクトバッファ
		buffers_[i].objectBuff_ = BufferResource::CreateBufferResource(device, ((sizeof(CourseCollisionSystem::ObjectData) + 0xff) & ~0xff));

		// 書き込むためのアドレスを取得
		buffers_[i].objectBuff_->Map(0, nullptr, reinterpret_cast<void**>(&(buffers_[i].objectMap_)));
		// 頂点データを初期化
		buffers_[i].objectMap_->center = { 0.0f,0.0f,0.0f };
		buffers_[i].objectMap_->otientatuonsX = { 1.0f, 0.0f, 0.0f };
		buffers_[i].objectMap_->otientatuonsY = { 0.0f, 1.0f, 0.0f };
		buffers_[i].objectMap_->otientatuonsZ = { 0.0f, 0.0f, 1.0f };
		buffers_[i].objectMap_->size = { 1.0f, 1.0f, 1.0f };
		buffers_[i].objectMap_->indexMax = 0;

		// 衝突するかもしれないポリゴンデータバッファ
		buffers_[i].polygonDataBuff_ = BufferResource::CreateBufferResource(device, ((sizeof(CoursePolygon) + 0xff) & ~0xff) * kCollisionPolygonMax_);
		// 書き込むためのアドレスを取得
		buffers_[i].polygonDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&buffers_[i].polygonDataMap_));

		// ポリゴンデータを初期化
		for (uint32_t j = 0; j < kCollisionPolygonMax_; ++j) {
			buffers_[i].polygonDataMap_[j].position0 = { 0.0f, 0.0f, 0.0f };
			buffers_[i].polygonDataMap_[j].position1 = { 0.0f, 0.0f, 0.0f };
			buffers_[i].polygonDataMap_[j].position2 = { 0.0f, 0.0f, 0.0f };
			buffers_[i].polygonDataMap_[j].normal = { 0.0f, 0.0f, 0.0f };
			buffers_[i].polygonDataMap_[j].texcoord = { 0.0f, 0.0f };
		}

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		srvDesc.Buffer.NumElements = static_cast<UINT>(kCollisionPolygonMax_);
		srvDesc.Buffer.StructureByteStride = sizeof(CoursePolygon);
		buffers_[i].polygonDataDescriptorHandles.handleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
		buffers_[i].polygonDataDescriptorHandles.handleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
		buffers_[i].polygonDataDescriptorHandles.descriptorHeap = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
		SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
		device->CreateShaderResourceView(buffers_[i].polygonDataBuff_.Get(), &srvDesc, buffers_[i].polygonDataDescriptorHandles.handleCPU_);


		// 押し出しと走行場所のデータ（UAV）
		buffers_[i].outputDataBuff_ = BufferResource::CreateBufferResourceMapUAV(device, ((sizeof(CourseCollisionSystem::OutputData) + 0xff) & ~0xff) * kCollisionPolygonMax_);
		// 書き込むためのアドレスを取得
		buffers_[i].outputDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&buffers_[i].outputDataMap_));
		
		// ポリゴンデータを初期化
		for (uint32_t j = 0; j < kCollisionPolygonMax_; ++j) {
			buffers_[i].outputDataMap_[j].extrusion = { 0.0f,0.0f,0.0f };
			buffers_[i].outputDataMap_[j].drivingLocation = static_cast<uint32_t>(kCoursePolygonTypeRoad);
			buffers_[i].outputDataMap_[j].collided = 0;
		}

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = static_cast<UINT>(kCollisionPolygonMax_);
		uavDesc.Buffer.CounterOffsetInBytes = 0;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
		uavDesc.Buffer.StructureByteStride = sizeof(CourseCollisionSystem::OutputData);

		buffers_[i].outputDescriptorHandles.handleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
		buffers_[i].outputDescriptorHandles.handleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
		buffers_[i].outputDescriptorHandles.descriptorHeap = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
		SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

		device->CreateUnorderedAccessView(buffers_[i].outputDataBuff_.Get(), nullptr, &uavDesc, buffers_[i].outputDescriptorHandles.handleCPU_);

	}

}

void CourseCollisionSystem::DistanceJudgment(CollisionObject object)
{

	// オブジェクト情報
	Vector3 objectPosition = { 0.0f,0.0f,0.0f };

	// obb取得
	OBB obb = std::visit([&](auto x) {
		// 型
		using T = std::decay_t<decltype(x)>;
		// プレイヤー
		if constexpr (std::is_same_v<T, Player*>) {
			return std::get<OBB>(*x->GetCollider());
		}
		// デモオブジェクト
		else if constexpr (std::is_same_v<T, CourseDemoObject*>) {
			return std::get<OBB>(*x->GetCollider());
		}
		// プレイヤーじゃない
		else {
			CollisionCarObject collisionCarObject = x;
			OBB obbXX = std::visit([&](auto xx) {
				return std::get<OBB>(*xx->GetCollider());
				}, collisionCarObject);
			return obbXX;
		}
		}, object);


	// オブジェクトデータ取得
	buffers_[collisionCheakNum_].objectMap_->center = obb.center_;
	// 軸を送る時の代入
	//軸反転応急措置byシマザキ
	buffers_[collisionCheakNum_].objectMap_->otientatuonsX = { 0.0f, 0.5f, 0.5f };
	buffers_[collisionCheakNum_].objectMap_->otientatuonsY = { 0.5f, 0.0f, 0.5f };
	buffers_[collisionCheakNum_].objectMap_->otientatuonsZ = { 0.5f, 0.5f, 0.0f };

	buffers_[collisionCheakNum_].objectMap_->size = obb.size_;

	// オブジェクトの位置からエリアを取得

	// 頂点8個
	const uint32_t vertexNum = 8;
	// 頂点求める
	Vector3 vertices[8];
	const ObjectData objectData = *buffers_[collisionCheakNum_].objectMap_;

	// 左 上 前
	vertices[0] = {
		(-objectData.size.x * objectData.otientatuonsX.x + objectData.size.y * objectData.otientatuonsY.x +
			-objectData.size.z * objectData.otientatuonsZ.x),
		(-objectData.size.x * objectData.otientatuonsX.y + objectData.size.y * objectData.otientatuonsY.y +
			-objectData.size.z * objectData.otientatuonsZ.y),
		(-objectData.size.x * objectData.otientatuonsX.z + objectData.size.y * objectData.otientatuonsY.z +
			-objectData.size.z * objectData.otientatuonsZ.z)
	};

	// 左 上 後
	vertices[1] = {
		(-objectData.size.x * objectData.otientatuonsX.x + objectData.size.y * objectData.otientatuonsY.x +
			objectData.size.z * objectData.otientatuonsZ.x),
		(-objectData.size.x * objectData.otientatuonsX.y + objectData.size.y * objectData.otientatuonsY.y +
			objectData.size.z * objectData.otientatuonsZ.y),
		(-objectData.size.x * objectData.otientatuonsX.z + objectData.size.y * objectData.otientatuonsY.z +
			objectData.size.z * objectData.otientatuonsZ.z)
	};

	// 右 上 前
	vertices[2] = {
		(objectData.size.x * objectData.otientatuonsX.x + objectData.size.y * objectData.otientatuonsY.x +
			-objectData.size.z * objectData.otientatuonsZ.x),
		(objectData.size.x * objectData.otientatuonsX.y + objectData.size.y * objectData.otientatuonsY.y +
			-objectData.size.z * objectData.otientatuonsZ.y),
		(objectData.size.x * objectData.otientatuonsX.z + objectData.size.y * objectData.otientatuonsY.z +
			-objectData.size.z * objectData.otientatuonsZ.z)
	};

	// 右 上 後
	vertices[3] = {
		(objectData.size.x * objectData.otientatuonsX.x + objectData.size.y * objectData.otientatuonsY.x +
			objectData.size.z * objectData.otientatuonsZ.x),
		(objectData.size.x * objectData.otientatuonsX.y + objectData.size.y * objectData.otientatuonsY.y +
			objectData.size.z * objectData.otientatuonsZ.y),
		(objectData.size.x * objectData.otientatuonsX.z + objectData.size.y * objectData.otientatuonsY.z +
			objectData.size.z * objectData.otientatuonsZ.z)
	};

	// 左 下 前
	vertices[4] = {
		(-objectData.size.x * objectData.otientatuonsX.x + -objectData.size.y * objectData.otientatuonsY.x +
			-objectData.size.z * objectData.otientatuonsZ.x),
		(-objectData.size.x * objectData.otientatuonsX.y + -objectData.size.y * objectData.otientatuonsY.y +
			-objectData.size.z * objectData.otientatuonsZ.y),
		(-objectData.size.x * objectData.otientatuonsX.z + -objectData.size.y * objectData.otientatuonsY.z +
			-objectData.size.z * objectData.otientatuonsZ.z)
	};

	// 左 下 後
	vertices[5] = {
		(-objectData.size.x * objectData.otientatuonsX.x + -objectData.size.y * objectData.otientatuonsY.x +
			objectData.size.z * objectData.otientatuonsZ.x),
		(-objectData.size.x * objectData.otientatuonsX.y + -objectData.size.y * objectData.otientatuonsY.y +
			objectData.size.z * objectData.otientatuonsZ.y),
		(-objectData.size.x * objectData.otientatuonsX.z + -objectData.size.y * objectData.otientatuonsY.z +
			objectData.size.z * objectData.otientatuonsZ.z)
	};

	// 右 下 前
	vertices[6] = {
		(objectData.size.x * objectData.otientatuonsX.x + -objectData.size.y * objectData.otientatuonsY.x +
			-objectData.size.z * objectData.otientatuonsZ.x),
		(objectData.size.x * objectData.otientatuonsX.y + -objectData.size.y * objectData.otientatuonsY.y +
			-objectData.size.z * objectData.otientatuonsZ.y),
		(objectData.size.x * objectData.otientatuonsX.z + -objectData.size.y * objectData.otientatuonsY.z +
			-objectData.size.z * objectData.otientatuonsZ.z)
	};

	// 右 下 後
	vertices[7] = {
		(objectData.size.x * objectData.otientatuonsX.x + -objectData.size.y * objectData.otientatuonsY.x +
			objectData.size.z * objectData.otientatuonsZ.x),
		(objectData.size.x * objectData.otientatuonsX.y + -objectData.size.y * objectData.otientatuonsY.y +
			objectData.size.z * objectData.otientatuonsZ.y),
		(objectData.size.x * objectData.otientatuonsX.z + -objectData.size.y * objectData.otientatuonsY.z +
			objectData.size.z * objectData.otientatuonsZ.z)
	};

	for (uint32_t i = 0; i < 8; ++i)
	{
		vertices[i] += objectData.center;

	}

	// エリア8個
	int32_t x[vertexNum] = {};
	int32_t y[vertexNum] = {};
	int32_t z[vertexNum] = {};
	
	// 初期化
	for (uint32_t i = 0; i < vertexNum; ++i) {
		x[i] = -1;
		y[i] = -1;
		z[i] = -1;
	}

	// 割る用の値
	Vector3 dividingValue = Vector3::Multiply(kPolygonAreasLength_, 1.0f / static_cast<float>(kPolygonAreasDiv_));

	// エリアに入っているポリゴンデータをSRVに登録
	uint32_t polygonDataMapIndex = 0;
	for (uint32_t i = 0; i < vertexNum; ++i) {

		// 現在の値のエリア番号
		int32_t tmpX = static_cast<int32_t>((vertices[i].x - kPolygonAreasOrigin_.x) / dividingValue.x);
		int32_t tmpY = static_cast<int32_t>((vertices[i].y - kPolygonAreasOrigin_.y) / dividingValue.y);
		int32_t tmpZ = static_cast<int32_t>((vertices[i].z - kPolygonAreasOrigin_.z) / dividingValue.z);

		// エリア範囲外
		if (tmpX >= kPolygonAreasDiv_ || tmpY >= kPolygonAreasDiv_ || tmpZ >= kPolygonAreasDiv_ ||
			tmpX <= -1 || tmpY <= -1 || tmpZ <= -1) {
			continue;
		}

		// かぶり確認
		bool fogging = false;
		for (uint32_t j = 0; j < i; ++j) {
			if (tmpX == x[j] && tmpY == y[j] && tmpZ == z[j]) {
				fogging = true;
				break;
			}
		}

		// かぶっていないので登録
		if (!fogging) {

			// ポリゴン登録
			for (uint32_t j = 0; j < polygonAreas[tmpX][tmpY][tmpZ].size(); ++j) {
				buffers_[collisionCheakNum_].polygonDataMap_[polygonDataMapIndex] = polygonAreas[tmpX][tmpY][tmpZ][j];
				polygonDataMapIndex++;
			}

		}
		
		// かぶり確認用追加
		x[i] = tmpX;
		y[i] = tmpY;
		z[i] = tmpZ;

	}

	buffers_[collisionCheakNum_].objectMap_->indexMax = polygonDataMapIndex;

}

void CourseCollisionSystem::ExtrusionExecuteCS()
{

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommadList();

	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	courseCollisionPipeline_->SetPipelineState(commandList);

	commandList->SetComputeRootConstantBufferView(0, buffers_[collisionCheakNum_].objectBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(1, buffers_[collisionCheakNum_].polygonDataDescriptorHandles.handleGPU_);

	commandList->SetComputeRootDescriptorTable(2, buffers_[collisionCheakNum_].outputDescriptorHandles.handleGPU_);

	TextureManager::GetInstance()->SetComputeRootDescriptorTable(commandList, 3, course_->GetCourseTextureHandle());

	commandList->Dispatch((buffers_[collisionCheakNum_].objectMap_->indexMax / 1024) + 1, 1, 1);

}

void CourseCollisionSystem::CommadKick()
{

	//コマンドリストをクローズ、キック
	HRESULT hr = dxCommon_->GetCommadList()->Close();
	assert(SUCCEEDED(hr));

	ID3D12CommandList* commandLists[] = { dxCommon_->GetCommadList() };
	DxCommand::GetCommandQueue()->ExecuteCommandLists(1, commandLists);

	//実行待ち
	//Fenceの値を更新
	dxCommon_->SetFenceVal(dxCommon_->GetFenceVal() + 1);
	//GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	DxCommand::GetCommandQueue()->Signal(dxCommon_->GetFence(), dxCommon_->GetFenceVal());

	//Fenceの値が指定したSignal値にたどり着いているが確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (dxCommon_->GetFence()->GetCompletedValue() < dxCommon_->GetFenceVal()) {
		//FrenceのSignalを持つためのイベントを作成する
		HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fenceEvent != nullptr);
		//指定したSignalにたどりついていないので、たどりつくまで待つようにイベントを設定する
		dxCommon_->GetFence()->SetEventOnCompletion(dxCommon_->GetFenceVal(), fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	//実行が完了したので、アロケータとコマンドリストをリセット
	hr = dxCommon_->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(hr));
	hr = dxCommon_->GetCommadList()->Reset(dxCommon_->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(hr));

}

void CourseCollisionSystem::AloneExtrusionCalculation(CollisionObject object)
{

	// 押し出し
	Vector3 extrusion = { 0.0f,0.0f,0.0f };
	Vector3 extrusionWall = { 0.0f,0.0f,0.0f };
	Vector3 extrusionRoad = { 0.0f,0.0f,0.0f };
	uint32_t extrusionWallCount = 0;
	uint32_t extrusionRoadCount = 0;
	// 法線
	Vector3 normal = { 0.0f,0.0f,0.0f };
	uint32_t normalCount = 0;

	// ポリゴン数だけ回す
	for (uint32_t i = 0; i < buffers_[collisionCheakNum_].objectMap_->indexMax; i++) {

		// 出力データ
		OutputData outputData =	buffers_[collisionCheakNum_].outputDataMap_[i];

		// 衝突したか
		if (outputData.collided == 1) {
			// 壁ではないなら
			if (outputData.drivingLocation < CoursePolygonType::kCoursePolygonTypeWall) {
				// 押し出し値
				extrusionRoad += outputData.extrusion;
				extrusionRoadCount++;
				// 法線
				normal += Vector3::Normalize(outputData.extrusion);
				normalCount++;

			}
			else {
				extrusionWall += outputData.extrusion;
				extrusionWallCount++;
			}

		}

	}

	//押し出し

	if (extrusionRoadCount > 0) {
		extrusion += extrusionRoad * (1.0f / static_cast<float>(extrusionRoadCount));
	}
	if (extrusionWallCount > 0) {
		extrusion += extrusionWall * (1.0f / static_cast<float>(extrusionWallCount));
	}

	// 法線
	if (normalCount == 0 || Vector3::Length(normal) == 0.0f) {
		normal = { 0.0f, 0.0f, 1.0f };
	}
	else {
		normal = Vector3::Normalize(normal * (1.0f / static_cast<float>(normalCount)));
	}

	normal = { 0.0f, 0.0f, 1.0f };

	// メッシュオブジェクトに代入
	std::visit([&](auto x) {
		// 型
		using T = std::decay_t<decltype(x)>;
		// プレイヤー
		if constexpr (std::is_same_v<T, Player*>) {
			x->GetWorldTransformAdress()->transform_.translate += extrusion;
			if (normalCount != 0) {
				x->GetWorldTransformAdress()->direction_ = normal;
			}
			x->GetWorldTransformAdress()->UpdateMatrix();
		}
		// デモオブジェクト
		else if constexpr (std::is_same_v<T, CourseDemoObject*>) {
			x->GetWorldTransformAdress()->transform_.translate += extrusion;
			if (normalCount != 0) {
				x->GetWorldTransformAdress()->direction_ = normal;
			}
			x->GetWorldTransformAdress()->UpdateMatrix();
		}
		// プレイヤーじゃない
		else {
			CollisionCarObject collisionCarObject = x;
			std::visit([&](auto xx) {
				xx->GetWorldTransformAdress()->transform_.translate += extrusion;
				if (normalCount != 0) {
					xx->GetWorldTransformAdress()->direction_ = normal;
				}
				xx->GetWorldTransformAdress()->UpdateMatrix();
				}, collisionCarObject);
		}
		}, object);

}

void CourseCollisionSystem::CartExtrusionCalculation()
{

	// コアがない
	if (!vehicleCore_) {
		return;
	}

	// 押し出し
	Vector3 extrusion = { 0.0f,0.0f,0.0f };
	Vector3 extrusionWall = { 0.0f,0.0f,0.0f };
	Vector3 extrusionRoad = { 0.0f,0.0f,0.0f };
	uint32_t extrusionWallCount = 0;
	uint32_t extrusionRoadCount = 0;
	// 法線
	Vector3 normal = { 0.0f,0.0f,0.0f };
	uint32_t normalCount = 0;
	// 走行場所
	CoursePolygonType drivingLocation = CoursePolygonType::kCoursePolygonTypeRoad;

	// カートに属しているパーツ&&衝突しているパーツの番号分回す
	for (std::list<uint32_t>::iterator itr = belongsToCartPartsNumbers_.begin();
		itr != belongsToCartPartsNumbers_.end(); ++itr) {

		// ポリゴン数だけ回す
		for (uint32_t i = 0; i < buffers_[*itr].objectMap_->indexMax; i++) {

			// 出力データ
			OutputData outputData = buffers_[*itr].outputDataMap_[i];

			// 衝突したか
			if (outputData.collided == 1) {
				// 壁ではないなら
				if (outputData.drivingLocation < CoursePolygonType::kCoursePolygonTypeWall) {
					// 押し出し値
					extrusionRoad += outputData.extrusion;
					extrusionRoadCount++;
					// 法線
					normal += Vector3::Normalize(outputData.extrusion);
					normalCount++;
					// 走行場所
					if (drivingLocation < static_cast<CoursePolygonType>(outputData.drivingLocation)) {
						drivingLocation = static_cast<CoursePolygonType>(outputData.drivingLocation);
					}

				}
				else {
					extrusionWall += outputData.extrusion;
					extrusionWallCount++;
				}

			}
		}

	}

	//押し出し

	if (extrusionRoadCount > 0) {
		extrusion += extrusionRoad * (1.0f / static_cast<float>(extrusionRoadCount));
	}
	if (extrusionWallCount > 0) {
		extrusion += extrusionWall * (1.0f / static_cast<float>(extrusionWallCount));
	}

	// 法線
	if (normalCount == 0) {
		normal = { 0.0f, 0.0f, 1.0f };
	}
	else {
		normal = Vector3::Normalize(normal * (1.0f / static_cast<float>(normalCount)));
	}

	// コアに代入
	vehicleCore_->GetWorldTransformAdress()->transform_.translate += extrusion;
	//if (normalCount != 0) {
	//	vehicleCore_->GetWorldTransformAdress()->direction_ = normal;
	//}
	vehicleCore_->GetWorldTransformAdress()->UpdateMatrix();

}

void CourseCollisionSystem::ObjectRegistrationPlayer(Player* player)
{

	// 型
	const std::type_info& t = typeid(*(player->GetStateMachine()->GetCurrentState()));
	std::string name = t.name();
	if (!(name == "class PlayerInVehicleState")) {
		collidingObjects_.push_back(player);
	}

}
