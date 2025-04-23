#include "PlayerInVehicleState.h"
#include "../../Player.h"

void PlayerInVehicleState::Initialize()
{
	player_->SetParent();
}

void PlayerInVehicleState::Update()
{

}

void PlayerInVehicleState::Exit()
{
	// 親子関係解除＋座標修正
	Vector3 wolrdPosition = player_->GetWorldTransformAdress()->GetWorldPosition();
	player_->GetWorldTransformAdress()->SetParent(nullptr);
	player_->GetWorldTransformAdress()->transform_.translate = wolrdPosition;
}
