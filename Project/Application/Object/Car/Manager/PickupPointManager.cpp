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
