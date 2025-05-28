#include "PlayerInVehicleState.h"
#include "../../Player.h"
#include "../../../Car/VehicleCore.h"
#include "../../../Engine/Math/DeltaTime.h"

void PlayerInVehicleState::Initialize()
{
	player_->SettingParent();
	// 回転角初期化
	player_->GetWorldTransformAdress()->transform_.rotate = {};
}

void PlayerInVehicleState::Update()
{
	// 切り替え
	if (player_->GetCommand()->ActionCommand() && !player_->GetPickUpManager()->IsPartsHold()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kDropOffAction);
	}

	//player_->GetCommand()->InVehicleRotateCommand();
	// 移動処理
	//Vector3 velocityDirection = player_->GetCommand()->GetDirect() * PlayerDebugData::sMoveData.rideSpeed;
	// 車両への入力処理
	player_->GetCore()->GetDriveSystem()->InputAccept(player_->GetCommand()->GetKeyConfig());
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
