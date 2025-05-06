#include "PlayerInVehicleState.h"
#include "../../Player.h"
#include "../../../Engine/Math/DeltaTime.h"

void PlayerInVehicleState::Initialize()
{
	player_->SetParent();
}

void PlayerInVehicleState::Update()
{
	// 切り替え
	if (player_->GetCommand()->ActionCommand()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kOnFoot);
	}

	player_->GetCommand()->InVehicleRotateCommand();
	// 移動処理
	Vector3 velocityDirection = player_->GetCommand()->GetDirect() * PlayerDebugData::sMoveData.rideSpeed;
	player_->GetWorldTransformAdress()->parent_->transform_.translate += velocityDirection * kDeltaTime_;
}

void PlayerInVehicleState::Exit()
{
	// 親子関係解除＋座標修正
	Vector3 wolrdPosition = player_->GetWorldTransformAdress()->GetWorldPosition();
	player_->GetWorldTransformAdress()->SetParent(nullptr);
	player_->GetWorldTransformAdress()->transform_.translate = wolrdPosition;
}
