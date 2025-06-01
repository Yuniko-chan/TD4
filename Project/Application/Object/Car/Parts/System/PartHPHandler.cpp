#include "PartHPHandler.h"
#include "../PartsInterface.h"

void PartHPHandler::Initialize()
{

	hp_ = 1;

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
