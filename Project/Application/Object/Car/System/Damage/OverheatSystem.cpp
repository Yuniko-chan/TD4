#include "OverheatSystem.h"
#include "../../VehicleCore.h"

void OverheatSystem::Update()
{
	// オーバーヒートしてなければ早期
	if (!owner_->GetStatus()->GetIsOverheat()) {
		return;
	}

	// パーツにダメージを与えるメソッド
	DamageProcess();

}

void OverheatSystem::DamageProcess()
{
	//Car::IParts* targetPart = nullptr;
	std::vector<Car::IParts*> depths = {};
	int maxDepth = 0;
	// 最大深度値検索
	for (std::map<Vector2Int, Car::IParts*>::iterator it = this->mappingData_->begin();
		it != mappingData_->end(); ++it) {
		if (maxDepth < (*it).first.GetLength()) {
			maxDepth = (*it).first.GetLength();

		}

	}

	// 対象をリスト化
	for (std::map<Vector2Int, Car::IParts*>::iterator it = this->mappingData_->begin();
		it != mappingData_->end(); ++it) {
		if (maxDepth == (*it).first.GetLength()) {
			depths.push_back((*it).second);
		}
	}

	for (std::vector<Car::IParts*>::iterator it = depths.begin(); it != depths.end(); ++it) {
		(*it)->GetHPHandler()->HeatDamage();
	}

}
