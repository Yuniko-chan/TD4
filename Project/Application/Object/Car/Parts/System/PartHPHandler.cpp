#include "PartHPHandler.h"
#include "../PartsInterface.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Math/Ease.h"

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
	float t = hp_ / (float)maxHP_;
	float r = 1.0f;
	float g = Ease::Easing(Ease::EaseName::Lerp, 0.0f, 1.0f, t);
	float b = Ease::Easing(Ease::EaseName::Lerp, 0.0f, 1.0f, t);
	float a = 1.0f;
	Vector4 color = {};
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;
	owner_->SetMaterialColor(color);
	// HPチェック
	if (hp_ <= 0) {
		// 解除フラグ
		owner_->SetIsDelete(true);
		isDead_ = true;
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

void PartHPHandler::HeatDamage(float damage)
{
	//const int persent = 50;
	hp_ -= (damage) * kDeltaTime_;
	// 0より小さい値にならないように
	if (hp_ < 0) {
		hp_ = 0;
	}
}

bool PartHPHandler::IsDead()
{
	return owner_->GetIsDelete() && isDead_;
}
