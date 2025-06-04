#include "PartHPHandler.h"
#include "../PartsInterface.h"

void PartHPHandler::Initialize()
{

	hp_ = 1;
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
}

void PartHPHandler::OnHit(int16_t damage)
{
	hp_ -= damage;
	// 0より小さい値にならないように
	if (hp_ < 0) {
		hp_ = 0;
	}
}
