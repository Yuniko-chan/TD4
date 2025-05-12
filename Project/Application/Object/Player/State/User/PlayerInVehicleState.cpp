#include "PlayerInVehicleState.h"
#include "../../Player.h"
#include "../../../Engine/Math/DeltaTime.h"

void PlayerInVehicleState::Initialize()
{
	player_->SetParent();
	// 回転角初期化
	player_->GetWorldTransformAdress()->transform_.rotate = {};
}

void PlayerInVehicleState::Update()
{
	// 切り替え
	if (player_->GetCommand()->ActionCommand() && !player_->GetPickUpManager()->IsPartsHold()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kDropOffAction);
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
	Vector3 parentRotate = player_->GetWorldTransformAdress()->parent_->transform_.rotate;
	player_->GetWorldTransformAdress()->SetParent(nullptr);
	player_->GetWorldTransformAdress()->transform_.translate = wolrdPosition;
	player_->GetWorldTransformAdress()->transform_.rotate.y = parentRotate.y;
}
