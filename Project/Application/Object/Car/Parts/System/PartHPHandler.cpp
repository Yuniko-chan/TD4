#include "PartHPHandler.h"
#include "../PartsInterface.h"
#include "../../../../../Engine/Math/DeltaTime.h"

void PartHPHandler::Initialize()
{
	maxHP_ = 20;
	hp_ = maxHP_;
	// 死亡・解除状態をリセット
	owner_->SetIsDelete(false);
	owner_->SetIsDead(false);

}

void PartHPHandler::Update()
{
	// HPチェック
	if (hp_ <= 0) {
		// 解除フラグ
		owner_->SetIsDelete(true);
	}

	// 無敵経過
	InvisibleProgress();

}

void PartHPHandler::OnHit(float damage)
{

	// 無敵状態なのでリターン
	if (isInvisible_) {
		return;
	}
	else {
		isInvisible_ = true;
		invisibleCooltime_ = 0.0f;
	}

	hp_ -= damage;
	// 0より小さい値にならないように
	if (hp_ < 0) {
		hp_ = 0;
	}

}

void PartHPHandler::InvisibleProgress()
{

	// 無敵か
	if (isInvisible_) {
		// 経過
		invisibleCooltime_ += kDeltaTime_;
		// 無敵最大時間
		const float kInvisibleTimeMax = 1.0f;
		// 無敵終了か
		if (invisibleCooltime_ >= kInvisibleTimeMax) {
			isInvisible_ = false;
		}
	}

}

void PartHPHandler::HeatDamage()
{
	//const int persent = 50;
	hp_ -= (1.0f) * kDeltaTime_;
	// 0より小さい値にならないように
	if (hp_ < 0) {
		hp_ = 0;
	}
}
