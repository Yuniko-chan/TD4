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
#include "../Object/CustomArea/CustomArea.h"
#include <typeinfo>

// 衝突するオブジェクトキーワード
const std::array<std::string, CourseCollisionSystem::kCollidingObjectKeywordsMax_> 
CourseCollisionSystem::kCollidingObjectKeywords_ = 
{
	"class CourseDemoObject", // コースデモオブジェクト
	"class Player", // プレイヤー
	"class VehicleCore", // ベーシックコア
	"class TireParts", // タイヤ
	"class ArmorFrameParts", // フレーム
	"class EngineParts" // エンジン
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

	// テクスチャハンドル
	roadAttributeTextureHandle_ = TextureManager::Load("Resources/Course/course.png", dxCommon_);

	// ポリゴン登録番号
	polygonRegistrationNumber_ = 0;

	// ポリゴン
	for (size_t i = 0; i < kCollisionPolygonMax_; ++i) {
		polygons_[i].position0 = { 0.0f,-10000.0f,0.0f };
		polygons_[i].position1 = { 0.0f,-10000.0f,0.0f };
		polygons_[i].position2 = { 0.0f,-10000.0f,0.0f };
		polygons_[i].normal = { 0.0f,0.0f,1.0f };
		polygons_[i].texcoord = { 0.0f,0.0f };
	}

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
		bool isAlone = true;
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
					// コア
					if(xx->GetParent()) {
						isAlone = false;
						return true;
					}
					return false;
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
		else if(isAlone){
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

		IObject* obj = itr->second.get();

		// 型
		const std::type_info& t = typeid(*obj);
		std::string className = t.name();

		// キーワード検索
		for (uint32_t i = 0; i < kCollidingObjectKeywordsMax_; ++i) {
			// キーワードに引っかかるか
			if (className == kCollidingObjectKeywords_[i]) {
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

	// コースメッシュ分回す
	std::vector<CoursePolygon>* polygons = course->GetCoursePolygonsAdress();
	for (uint32_t i = 0; i < polygons->size(); ++i) {

		// ポリゴン
		CoursePolygon polygon = (*polygons)[i];

		// コース中心のワールド座標をプラス
		polygon.position0 = Matrix4x4::Transform(polygon.position0, course->GetWorldTransformAdress()->worldMatrix_);
		polygon.position1 = Matrix4x4::Transform(polygon.position1, course->GetWorldTransformAdress()->worldMatrix_);
		polygon.position2 = Matrix4x4::Transform(polygon.position2, course->GetWorldTransformAdress()->worldMatrix_);

		// 登録
		polygons_[polygonRegistrationNumber_] = polygon;
		polygonRegistrationNumber_ = (polygonRegistrationNumber_ + 1) % kCollisionPolygonMax_;

	}

}

void CourseCollisionSystem::SetCustomArea(CustomArea* customArea)
{

	// ポリゴン
	CoursePolygon polygon = {};
	polygon.normal = { 0.0f,0.0f,1.0f };
	polygon.texcoord = { 0.0f,0.0f };

	// 高さ
	const float kHeight = 0.25f;
	// 幅
	const float kWidth = 20.0f;
	
	polygon.position0 = { -kWidth, kHeight, -kWidth };
	polygon.position1 = { -kWidth, kHeight, kWidth };
	polygon.position2 = { kWidth, kHeight, kWidth };

	// コース中心のワールド座標をプラス
	polygon.position0 = Matrix4x4::Transform(polygon.position0, customArea->GetWorldTransformAdress()->worldMatrix_);
	polygon.position1 = Matrix4x4::Transform(polygon.position1, customArea->GetWorldTransformAdress()->worldMatrix_);
	polygon.position2 = Matrix4x4::Transform(polygon.position2, customArea->GetWorldTransformAdress()->worldMatrix_);

	// 登録
	polygons_[polygonRegistrationNumber_] = polygon;
	polygonRegistrationNumber_ = (polygonRegistrationNumber_ + 1) % kCollisionPolygonMax_;

	polygon.position0 = { -kWidth, kHeight, -kWidth };
	polygon.position1 = { kWidth, kHeight, kWidth };
	polygon.position2 = { kWidth, kHeight, -kWidth };

	// コース中心のワールド座標をプラス
	polygon.position0 = Matrix4x4::Transform(polygon.position0, customArea->GetWorldTransformAdress()->worldMatrix_);
	polygon.position1 = Matrix4x4::Transform(polygon.position1, customArea->GetWorldTransformAdress()->worldMatrix_);
	polygon.position2 = Matrix4x4::Transform(polygon.position2, customArea->GetWorldTransformAdress()->worldMatrix_);

	// 登録
	polygons_[polygonRegistrationNumber_] = polygon;
	polygonRegistrationNumber_ = (polygonRegistrationNumber_ + 1) % kCollisionPolygonMax_;

}

void CourseCollisionSystem::SetGimmick(OBB* obb)
{

	// ポリゴン
	CoursePolygon polygon = {};
	polygon.normal = { 0.0f,0.0f,1.0f };
	polygon.texcoord = { 1.0f, 1.0f };
	
	// OBB平面作成
	Vector3 otientatuonX = obb->otientatuons_[0];
	Vector3 otientatuonY = obb->otientatuons_[1];
	Vector3 otientatuonZ = obb->otientatuons_[2];


	Vector3 size = obb->size_;
	Vector3 center = obb->center_;

	// 頂点求める
	Vector3 vertices[8];

	// 左 上 前
	vertices[0] = {
		(-size.x * otientatuonX.x + size.y * otientatuonY.x + -size.z * otientatuonZ.x),
		(-size.x * otientatuonX.y + size.y * otientatuonY.y + -size.z * otientatuonZ.y),
		(-size.x * otientatuonX.z + size.y * otientatuonY.z + -size.z * otientatuonZ.z)
	};

	// 左 上 後
	vertices[1] = {
		(-size.x * otientatuonX.x + size.y * otientatuonY.x + size.z * otientatuonZ.x),
		(-size.x * otientatuonX.y + size.y * otientatuonY.y + size.z * otientatuonZ.y),
		(-size.x * otientatuonX.z + size.y * otientatuonY.z + size.z * otientatuonZ.z)
	};

	// 右 上 前
	vertices[2] = {
		(size.x * otientatuonX.x + size.y * otientatuonY.x + -size.z * otientatuonZ.x),
		(size.x * otientatuonX.y + size.y * otientatuonY.y + -size.z * otientatuonZ.y),
		(size.x * otientatuonX.z + size.y * otientatuonY.z + -size.z * otientatuonZ.z)
	};

	// 右 上 後
	vertices[3] = {
		(size.x * otientatuonX.x + size.y * otientatuonY.x + size.z * otientatuonZ.x),
		(size.x * otientatuonX.y + size.y * otientatuonY.y + size.z * otientatuonZ.y),
		(size.x * otientatuonX.z + size.y * otientatuonY.z + size.z * otientatuonZ.z)
	};

	// 左 下 前
	vertices[4] = {
		(-size.x * otientatuonX.x + -size.y * otientatuonY.x + -size.z * otientatuonZ.x),
		(-size.x * otientatuonX.y + -size.y * otientatuonY.y + -size.z * otientatuonZ.y),
		(-size.x * otientatuonX.z + -size.y * otientatuonY.z + -size.z * otientatuonZ.z)
	};

	// 左 下 後
	vertices[5] = {
		(-size.x * otientatuonX.x + -size.y * otientatuonY.x + size.z * otientatuonZ.x),
		(-size.x * otientatuonX.y + -size.y * otientatuonY.y + size.z * otientatuonZ.y),
		(-size.x * otientatuonX.z + -size.y * otientatuonY.z + size.z * otientatuonZ.z)
	};

	// 右 下 前
	vertices[6] = {
		(size.x * otientatuonX.x + -size.y * otientatuonY.x + -size.z * otientatuonZ.x),
		(size.x * otientatuonX.y + -size.y * otientatuonY.y + -size.z * otientatuonZ.y),
		(size.x * otientatuonX.z + -size.y * otientatuonY.z + -size.z * otientatuonZ.z)
	};

	// 右 下 後
	vertices[7] = {
		(size.x * otientatuonX.x + -size.y * otientatuonY.x + size.z * otientatuonZ.x),
		(size.x * otientatuonX.y + -size.y * otientatuonY.y + size.z * otientatuonZ.y),
		(size.x * otientatuonX.z + -size.y * otientatuonY.z + size.z * otientatuonZ.z)
	};

	for (size_t i = 0; i < 8; ++i) {
		vertices[i] += center;
	}

	Vector3 planeVertices[4][4];

	// 前
	planeVertices[0][0] = vertices[4];
	planeVertices[0][1] = vertices[0];
	planeVertices[0][2] = vertices[6];
	planeVertices[0][3] = vertices[2];

	// 後
	planeVertices[1][0] = vertices[7];
	planeVertices[1][1] = vertices[3];
	planeVertices[1][2] = vertices[5];
	planeVertices[1][3] = vertices[1];

	// 右
	planeVertices[2][0] = vertices[6];
	planeVertices[2][1] = vertices[2];
	planeVertices[2][2] = vertices[7];
	planeVertices[2][3] = vertices[3];

	// 左
	planeVertices[3][0] = vertices[5];
	planeVertices[3][1] = vertices[1];
	planeVertices[3][2] = vertices[4];
	planeVertices[3][3] = vertices[0];

	for (uint32_t i = 0; i < 4; ++i) {
	
		// 法線
		Vector3 v01 = planeVertices[i][1] - planeVertices[i][0];
		Vector3 v12 = planeVertices[i][2] - planeVertices[i][1];
		Vector3 v20 = planeVertices[i][0] - planeVertices[i][2];

		// 法線
		polygon.normal = Vector3::Normalize(Vector3::Cross(v01, v12));
	
		polygon.position0 = planeVertices[i][0];
		polygon.position1 = planeVertices[i][1];
		polygon.position2 = planeVertices[i][3];
		
		// 登録
		polygons_[polygonRegistrationNumber_] = polygon;
		polygonRegistrationNumber_ = (polygonRegistrationNumber_ + 1) % kCollisionPolygonMax_;

		polygon.position0 = planeVertices[i][0];
		polygon.position1 = planeVertices[i][3];
		polygon.position2 = planeVertices[i][2];

		// 登録
		polygons_[polygonRegistrationNumber_] = polygon;
		polygonRegistrationNumber_ = (polygonRegistrationNumber_ + 1) % kCollisionPolygonMax_;

	}

}

void CourseCollisionSystem::ClearCorse()
{

	// ポリゴンをクリアする
	//polygons_.clear();

}

void CourseCollisionSystem::ImGuiDraw()
{

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
		buffers_[i].objectMap_->planeYZ = { 1.0f, 0.0f, 0.0f };
		buffers_[i].objectMap_->planeXZ = { 0.0f, 1.0f, 0.0f };
		buffers_[i].objectMap_->planeXY = { 0.0f, 0.0f, 1.0f };
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
	//buffers_[collisionCheakNum_].objectMap_->planeYZ = Vector3::Normalize(Vector3{ 1.0f - std::fabs(obb.otientatuons_[0].x), 1.0f - std::fabs(obb.otientatuons_[0].y), 1.0f - std::fabs(obb.otientatuons_[0].z) });
	//buffers_[collisionCheakNum_].objectMap_->planeXZ = Vector3::Normalize(Vector3{ 1.0f - std::fabs(obb.otientatuons_[1].x), 1.0f - std::fabs(obb.otientatuons_[1].y), 1.0f - std::fabs(obb.otientatuons_[1].z) });
	//buffers_[collisionCheakNum_].objectMap_->planeXY = Vector3::Normalize(Vector3{ 1.0f - std::fabs(obb.otientatuons_[2].x), 1.0f - std::fabs(obb.otientatuons_[2].y), 1.0f - std::fabs(obb.otientatuons_[2].z) });

	// 軸固定
	buffers_[collisionCheakNum_].objectMap_->planeYZ = Vector3{ 0.0f, 0.5f,0.5f };
	buffers_[collisionCheakNum_].objectMap_->planeXZ = Vector3{ 0.5f, 0.0f,0.5f };
	buffers_[collisionCheakNum_].objectMap_->planeXY = Vector3{ 0.5f, 0.5f,0.0f };


	buffers_[collisionCheakNum_].objectMap_->size = obb.size_;

	// ポリゴン登録
	for (uint32_t i = 0; i < polygons_.size(); ++i) {
		buffers_[collisionCheakNum_].polygonDataMap_[i] = polygons_[i];
	}

	buffers_[collisionCheakNum_].objectMap_->indexMax = static_cast<uint32_t>(polygons_.size());

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

	TextureManager::GetInstance()->SetComputeRootDescriptorTable(commandList, 3, roadAttributeTextureHandle_);

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

	// メッシュオブジェクトに代入
	std::visit([&](auto x) {
		// 型
		using T = std::decay_t<decltype(x)>;
		// プレイヤー
		if constexpr (std::is_same_v<T, Player*>) {
			x->GetWorldTransformAdress()->transform_.translate += extrusion;
			x->GetWorldTransformAdress()->UpdateMatrix();
		}
		// デモオブジェクト
		else if constexpr (std::is_same_v<T, CourseDemoObject*>) {
			x->GetWorldTransformAdress()->transform_.translate += extrusion;
			x->GetWorldTransformAdress()->UpdateMatrix();
		}
		// プレイヤーじゃない
		else {
			CollisionCarObject collisionCarObject = x;
			std::visit([&](auto xx) {
				xx->GetWorldTransformAdress()->transform_.translate += extrusion;
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
