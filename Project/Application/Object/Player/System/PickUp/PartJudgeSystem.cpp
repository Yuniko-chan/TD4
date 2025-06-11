#include "PartJudgeSystem.h"
#include "../../../Car/Manager/VehiclePartsManager.h"
#include "../../../Car/Manager/PickupPointManager.h"
#include "../../../Car/PickupPoint/InterfacePickupPoint.h"
#include "../../../Utility/Calc/TransformHelper.h"
#include "../../../GameObjectsList.h"
#include "../../PlayerSystemLists.h"


Car::IParts* PartJudgeSystem::GetCatchPart(VehiclePartsManager* partManager, PickupPointManager* pointManager)
{
	Vector3 worldPosition = owner_->GetWorldTransformAdress()->GetWorldPosition();
	// 一番近いポイント（生成箇所）
	IPickupPoint* nearPoint = pointManager->FindNearPoint(worldPosition);
	// 一番近いパーツ
	Car::IParts* nearParts = partManager->FindRootNonCoreParts(worldPosition);
	// 自分のコア
	Car::IParts* nearCore = (partManager->FindNearCoreParts(worldPosition));
	bool isEmpty = static_cast<VehicleCore*>(nearCore)->GetConstructionSystem()->IsEmpty();

	// コアに付けている中で一番近いパーツを検索
	Car::IParts* nearDockingPart = nullptr;
	if (owner_->GetCore()) {
		nearDockingPart = owner_->GetCore()->GetConstructionSystem()->FindNearPart(owner_->GetWorldTransformAdress()->GetWorldPosition());
	}

	// 何もついてないパーツ
	bool isCore = (isEmpty) && nearCore;
	// 拾う場所
	bool isPoint = nearPoint;
	// 落ちてるパーツ
	bool isParts = nearParts;
	// 付いてるパーツ
	bool isDockingParts = nearDockingPart;

	ConditionData data = {};
	// コア
	if (isCore) {
		data.object = nearCore;
		data.type = Type::Core;
		data.distance = TransformHelper::Vector3Distance(worldPosition,
			nearCore->GetWorldTransformAdress()->GetWorldPosition());
		objects_.push_back(data);
	}
	// 拾う場所
	if (isPoint) {
		data.object = nearPoint;
		data.type = Type::PickupPoint;
		data.distance = TransformHelper::Vector3Distance(worldPosition,
			nearPoint->GetWorldTransformAdress()->GetWorldPosition());
		objects_.push_back(data);
	}
	// 落ちてるパーツ
	if (isParts) {
		data.object = nearParts;
		data.type = Type::Dropped;
		data.distance = TransformHelper::Vector3Distance(worldPosition,
			nearParts->GetWorldTransformAdress()->GetWorldPosition());
		objects_.push_back(data);
	}
	// 付いてるパーツ
	if (isDockingParts) {
		data.object = nearDockingPart;
		data.distance = TransformHelper::Vector3Distance(worldPosition,
			nearDockingPart->GetWorldTransformAdress()->GetWorldPosition());
		data.type = Type::Docked;
		objects_.push_back(data);
	}

	// 一番近いものを取得
	data = NearSort();

	// 返し
	return GetCatchPart(pointManager, data);
}

PartJudgeSystem::ConditionData PartJudgeSystem::NearSort()
{
	// 無ければ
	if (objects_.empty()) {
		return ConditionData();
	}
	// 一つしかなければ
	if (objects_.size() == 1) {
		return objects_.front();
	}

	// ソート（昇順）
	//std::sort(objects_.begin(), objects_.end(), [](const auto& a, const auto& b) {
	//	return a.distance < b.distance;
	//});
	objects_.sort([](const auto& a, const auto& b) {
		return a.distance < b.distance;
		});

	// 一番近い
	return objects_.front();
}

Car::IParts* PartJudgeSystem::GetCatchPart(PickupPointManager* pointManager, ConditionData data)
{	
	// 返しの値
	Car::IParts* result = nullptr;
	// プレイヤーのワールド
	Vector3 worldPosition = owner_->GetWorldTransformAdress()->GetWorldPosition();
	// 対象オブジェクトのワールド
	Vector3 objectWorld = data.object->GetWorldTransformAdress()->GetWorldPosition();
	// 向き
	Vector3 toDirect = Vector3(objectWorld.x, 0, objectWorld.z) - Vector3(worldPosition.x, 0, worldPosition.z);
	toDirect = Vector3::Normalize(toDirect);

	// 前方でなければnull
	if (!owner_->GetFrontChecker()->FrontCheck(toDirect)) {
		return nullptr;
	}

	// 種類ごとの処理
	switch (data.type)
	{
	case Type::Dropped:	// 落ちてる
		result = static_cast<Car::IParts*>(data.object);
		break;

	case Type::Docked:	// 外れる
		result = static_cast<Car::IParts*>(data.object);
		owner_->GetCore()->GetConstructionSystem()->Detach(result);
		break;

	case Type::Core:	// コア落ちてる
		result = static_cast<Car::IParts*>(data.object);
		break;

	case Type::PickupPoint:	// 拾う
		if (!pointManager->IsAccept(worldPosition)) {
			return nullptr;
		}
		// パーツ取得
		result = pointManager->AttemptPartAcquisition();
		break;

	default:
		break;
	}

	// 返り値
	return result;
}
