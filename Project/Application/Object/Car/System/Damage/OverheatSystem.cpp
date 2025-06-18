#include "OverheatSystem.h"
#include "../../VehicleCore.h"
#include "../../../Engine/Math/RandomEngine.h"

void OverheatSystem::Update()
{
	// オーバーヒートしてなければ早期
	if (!owner_->GetStatus()->GetIsOverheat()) {
		return;
	}

	// 深度情報更新
	preDepth_ = maxDepth_;
	// 最大値の初期化
	maxDepth_ = 0;

	// 最大深度値検索
	for (std::map<Vector2Int, Car::IParts*>::iterator it = this->mappingData_->begin();
		it != mappingData_->end(); ++it) {
		if (maxDepth_ < (*it).first.GetLength()) {
			maxDepth_ = (*it).first.GetLength();
		}
	}
	// パーツにダメージを与えるメソッド
	DamageProcess();

}

void OverheatSystem::DamageProcess()
{
	// 前フレームの深度と最大深度が異なれば対象を変更する処理
	if (preDepth_ != maxDepth_) {
		AssignRandomOverheatPart();
	}
	// 対象パーツが外れて親が亡くなった時に対象を更新
	else if (!targetPart_->IsParent()) {
		AssignRandomOverheatPart();
	}

	// 対象にダメージ
	if (targetPart_) {
		targetPart_->GetHPHandler()->HeatDamage(owner_->GetStatus()->GetHeatDPS());
	}
}

void OverheatSystem::AssignRandomOverheatPart()
{
	// 対象のリセット
	targetPart_ = nullptr;
	// 対象
	std::vector<Car::IParts*> depths = {};
	// 最大深度
	int maxDepth = maxDepth_;
	// 最大深度の物をリスト化
	for (std::map<Vector2Int, Car::IParts*>::iterator it = this->mappingData_->begin();
		it != mappingData_->end(); ++it) {
		if (maxDepth == (*it).first.GetLength()) {
			depths.push_back((*it).second);
		}
	}
	// 番号をランダムで決定
	int targetIndex = 0;
	if (depths.size() > 0) {
		targetIndex = (int)RandomEngine::GetRandom(0, (float)depths.size() - 1);
	}
	// 番号のパーツを設定
	targetPart_ = depths[targetIndex];
}
