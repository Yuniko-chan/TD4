#include "PickupPointManager.h"
#include "../PickupPoint/InterfacePickupPoint.h"

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
