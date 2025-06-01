#include "PickupPointManager.h"
#include "../PickupPoint/InterfacePickupPoint.h"
#include "../../Manager/GameSceneObjectManager.h"
#include "../Preset/VehiclePaths.h"
#include "../Parts/PartsInterface.h"
#include "../../Utility/Calc/TransformHelper.h"

void PickupPointManager::AddPickupPoint(const std::string& tag, IPickupPoint* newPoint)
{
	// 要素があれば
	if (pointLists_.contains(tag)) {
		return;
	}
	// リストに追加
	pointLists_.emplace(tag, newPoint);
}

void PickupPointManager::DeletePickupPoint(const std::string& tag)
{
	// 要素が無ければ
	if (!pointLists_.contains(tag)) {
		return;
	}
	// 削除
	pointLists_.erase(tag);
}

IPickupPoint* PickupPointManager::FindNearPoint(const Vector3& position)
{
	// 最大設定
	float maxValue = FLT_MAX;
	IPickupPoint* targetPoint = nullptr;

	for (std::unordered_map<std::string, IPickupPoint*>::iterator it = pointLists_.begin();
		it != pointLists_.end(); ++it) {
		// 距離ベクトル
		Vector3 distance = (*it).second->GetWorldTransformAdress()->GetWorldPosition() - position;
		float distanceValue = float(std::sqrtf(std::powf(distance.x, 2) + std::powf(distance.y, 2) + std::powf(distance.z, 2)));
		// 距離が近いと更新
		if (distanceValue <= maxValue) {
			targetPoint = (*it).second;
			maxValue = distanceValue;
		}
	}

	return targetPoint;
}

Car::IParts* PickupPointManager::GenerateParts(int32_t partNum)
{
	Car::IParts* newPart = nullptr;
	std::string objectName = "";
	switch (partNum)
	{
	case IPickupPoint::kEngine:
		Car::SerialNumberGenerate::sSerialEngine++;
		objectName = "Engine" + std::to_string(Car::SerialNumberGenerate::sSerialEngine);
		static_cast<GameSceneObjectManager*>(objectManager_)->AddObject("EngineParts", objectName, sVehiclePaths[VehicleDatas::kEngine].first, sVehiclePaths[VehicleDatas::kEngine].second);

		break;
	case IPickupPoint::kTire:
		objectName = "Tire" + std::to_string(Car::SerialNumberGenerate::sSerialTire);
		Car::SerialNumberGenerate::sSerialTire++;
		static_cast<GameSceneObjectManager*>(objectManager_)->AddObject("TireParts", objectName, sVehiclePaths[VehicleDatas::kTire].first, sVehiclePaths[VehicleDatas::kTire].second);

		break;
	case IPickupPoint::kArmor:
		Car::SerialNumberGenerate::sSerialArmor++;
		objectName = "Armor" + std::to_string(Car::SerialNumberGenerate::sSerialArmor);
		static_cast<GameSceneObjectManager*>(objectManager_)->AddObject("ArmorFrameParts", objectName, sVehiclePaths[VehicleDatas::kArmor].first, sVehiclePaths[VehicleDatas::kArmor].second);

		break;
	default:
		break;
	}
	// 取得
	newPart = static_cast<Car::IParts*>(static_cast<GameSceneObjectManager*>(objectManager_)->GetObjectPointer(objectName));
	
	static_cast<GameSceneObjectManager*>(objectManager_)->GetPartsManager()->AddParts(objectName, newPart);
	return newPart;
}

Car::IParts* PickupPointManager::AttemptPartAcquisition()
{
	//// 一番近いのを検索
	//IPickupPoint* nearPoint = FindNearPoint(position);

	// パーツの生成
	Car::IParts* parts = GenerateParts(pickupPoint_->GetType());

	return parts;
}

bool PickupPointManager::IsAccept(const Vector3& position)
{
	// 初期化
	pickupPoint_ = nullptr;
	// 一番近いのを検索
	IPickupPoint* nearPoint = FindNearPoint(position);

	// XZ平面上の距離
	Vector3 from = Vector3(position.x, 0.0f, position.z);
	Vector3 to =
		Vector3(nearPoint->GetWorldTransformAdress()->GetWorldPosition().x, 0.0f,
			nearPoint->GetWorldTransformAdress()->GetWorldPosition().z);

	float distance = TransformHelper::Vector3Distance(from, to);
	const float threshold = 50.0f;

	// 閾値を越えている場合受付できない
	if (distance >= threshold) {
		return false;
	}
	// 書き換え
	pickupPoint_ = nearPoint;
	return true;
}
