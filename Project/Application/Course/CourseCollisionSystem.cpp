#include "CourseCollisionSystem.h"
#include <algorithm>
#include "../../Engine/base/BufferResource.h"
#include "../../Engine/base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "CoursePolygonType.h"

// ブロードフェーズで使用する、距離判定
const float CourseCollisionSystem::kDistanceJudgment_ = 20.0f;
// ポリゴンエリアの原点
const Vector3 CourseCollisionSystem::kPolygonAreasOrigin_ = { -10.0f, -10.0f, -10.0f };
// ポリゴンエリアの長さ
const Vector3 CourseCollisionSystem::kPolygonAreasLength_ = { 20.0f, 20.0f, 20.0f };


void CourseCollisionSystem::Initialize()
{

	// バッファ初期化
	BuffersInitialize();

}

void CourseCollisionSystem::Execute()
{
	
	// 回数を初期化
	collisionCheakNum_ = 0;

	// 登録分回す
	for (std::list<MeshObject*>::iterator itr = collidingObjects_.begin();
		itr != collidingObjects_.end();++itr) {
		// CPU側でOBBとの距離判定をとる->メッシュ登録
		DistanceJudgment(*itr);

		// ->GPU側で押し出し距離確認

		// 回数を増やす
		collisionCheakNum_++;


	}

	// ->キック

	// 登録分回す
	for (std::list<MeshObject*>::iterator itr = collidingObjects_.begin();
		itr != collidingObjects_.end(); ++itr) {
	
		// ->CPU側で押し出し、回転（壁データはとらない）
		// ->OBB登録のオブジェクトのワールドトランスフォーム更新
	
	}

	// 後処理

	// 登録したオブジェクトリストをクリア
	collidingObjects_.clear();

}

void CourseCollisionSystem::ObjectRegistration(MeshObject* object)
{

	// オブジェクトリストに登録
	collidingObjects_.push_back(object);

}

void CourseCollisionSystem::SetCourse(Course* course)
{

	// コース
	course_ = course;

	// ポリゴンエリアに登録
	int32_t x0 = 0, y0 = 0, z0 = 0;
	int32_t x1 = 0, y1 = 0, z1 = 0;
	int32_t x2 = 0, y2 = 0, z2 = 0;

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
		Vector3 vertex0 = polygon.positions[0] - kPolygonAreasOrigin_ - worldPosition;
		Vector3 vertex1 = polygon.positions[1] - kPolygonAreasOrigin_ - worldPosition;
		Vector3 vertex2 = polygon.positions[2] - kPolygonAreasOrigin_ - worldPosition;

		// エリア番号
		x0 = static_cast<uint32_t>(vertex0.x / dividingValue.x);
		y0 = static_cast<uint32_t>(vertex0.y / dividingValue.y);
		z0 = static_cast<uint32_t>(vertex0.z / dividingValue.z);

		x1 = static_cast<uint32_t>(vertex1.x / dividingValue.x);
		y1 = static_cast<uint32_t>(vertex1.y / dividingValue.y);
		z1 = static_cast<uint32_t>(vertex1.z / dividingValue.z);

		x2 = static_cast<uint32_t>(vertex2.x / dividingValue.x);
		y2 = static_cast<uint32_t>(vertex2.y / dividingValue.y);
		z2 = static_cast<uint32_t>(vertex2.z / dividingValue.z);

		// 登録（エリアをまたぐ場合、それぞれ登録）

		// 0番目
		polygonAreas[x0][y0][z0].push_back(polygon);

		// 1番目
		if (!(x0 == x1 && y0 == y1 && z0 == z1)) {
			polygonAreas[x1][y1][z1].push_back(polygon);
		}

		// 2番目
		if (!(x0 == x2 && y0 == y2 && z0 == z2) && !(x1 == x2 && y1 == y2 && z1 == z2)) {
			polygonAreas[x2][y2][z2].push_back(polygon);
		}

	}

}

void CourseCollisionSystem::ImGuiDraw()
{



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
		buffers_[i].objectBuff_->Map(0, nullptr, reinterpret_cast<void**>(&buffers_[i].objectMap_));
		// 頂点データを初期化
		buffers_[i].objectMap_->center = { 0.0f,0.0f,0.0f };
		buffers_[i].objectMap_->otientatuons[0] = { 1.0f, 0.0f, 0.0f };
		buffers_[i].objectMap_->otientatuons[1] = { 0.0f, 1.0f, 0.0f };
		buffers_[i].objectMap_->otientatuons[2] = { 0.0f, 0.0f, 1.0f };
		buffers_[i].objectMap_->size = { 1.0f, 1.0f, 1.0f };

		// 衝突するかもしれないポリゴンデータバッファ
		buffers_[i].polygonDataBuff_ = BufferResource::CreateBufferResource(device, ((sizeof(CoursePolygon) + 0xff) & ~0xff) * kCollisionPolygonMax_);
		// 書き込むためのアドレスを取得
		buffers_[i].polygonDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&buffers_[i].polygonDataMap_));

		// ポリゴンデータを初期化
		for (uint32_t j = 0; j < kCollisionPolygonMax_; ++j) {
			buffers_[i].polygonDataMap_[j].positions[0] = { 0.0f,0.0f,0.0f };
			buffers_[i].polygonDataMap_[j].positions[1] = { 0.0f, 0.0f, 0.0f };
			buffers_[i].polygonDataMap_[j].positions[2] = { 0.0f, 0.0f, 0.0f };
			buffers_[i].polygonDataMap_[j].normal = { 0.0f, 0.0f, 0.0f };
			buffers_[i].polygonDataMap_[j].coursePolygonType = static_cast<uint32_t>(kCoursePolygonTypeRoad);
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
		buffers_[i].outputDataBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(CourseCollisionSystem::OutputData) + 0xff) & ~0xff) * kCollisionPolygonMax_);
		// 書き込むためのアドレスを取得
		buffers_[i].outputDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&buffers_[i].outputDataMap_));
		
		// ポリゴンデータを初期化
		for (uint32_t j = 0; j < kCollisionPolygonMax_; ++j) {
			buffers_[i].outputDataMap_[j].extrusion = { 0.0f,0.0f,0.0f };
			buffers_[i].outputDataMap_[j].drivingLocation = static_cast<uint32_t>(kCoursePolygonTypeRoad);
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

void CourseCollisionSystem::DistanceJudgment(MeshObject* object)
{

	// オブジェクト情報
	Vector3 objectPosition = { 0.0f,0.0f,0.0f };
	object;

	// オブジェクトデータ取得
	buffers_[collisionCheakNum_].objectMap_->center = { 0.0f,0.0f,0.0f };
	buffers_[collisionCheakNum_].objectMap_->otientatuons[0] = { 1.0f, 0.0f, 0.0f };
	buffers_[collisionCheakNum_].objectMap_->otientatuons[1] = { 0.0f, 1.0f, 0.0f };
	buffers_[collisionCheakNum_].objectMap_->otientatuons[2] = { 0.0f, 0.0f, 1.0f };
	buffers_[collisionCheakNum_].objectMap_->size = { 1.0f, 1.0f, 1.0f };

	// オブジェクトの位置からエリアを取得

	// 頂点8個
	const uint32_t vertexNum = 8;
	Vector3 vertexs[vertexNum] = {};
	// エリア8個
	int32_t x[vertexNum] = {};
	int32_t y[vertexNum] = {};
	int32_t z[vertexNum] = {};
	
	// 初期化
	for (uint32_t i = 0; i < vertexNum; ++i) {
		vertexs[i] = { 0.0f,0.0f,0.0f };
		x[i] = -1;
		y[i] = -1;
		z[i] = -1;
	}

	// 割る用の値
	Vector3 dividingValue = Vector3::Multiply(kPolygonAreasLength_, 1.0f / static_cast<float>(kPolygonAreasDiv_));

	// 現在の値のエリア番号
	int32_t tmpX = static_cast<uint32_t>(objectPosition.x / dividingValue.x);
	int32_t tmpY = static_cast<uint32_t>(objectPosition.y / dividingValue.y);
	int32_t tmpZ = static_cast<uint32_t>(objectPosition.z / dividingValue.z);

	// エリアに入っているポリゴンデータをSRVに登録
	uint32_t polygonDataMapIndex = 0;
	for (uint32_t i = 0; i < vertexNum; ++i) {

		// かぶり確認
		bool fogging = false;
		for (uint32_t j = 0; j < i; ++j) {
			if (tmpX == x[i] && tmpY == y[i] && tmpZ == z[i]) {
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

}
