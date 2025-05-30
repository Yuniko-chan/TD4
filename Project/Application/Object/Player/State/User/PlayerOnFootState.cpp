#include "PlayerOnFootState.h"
#include "../../Player.h"

#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"

void PlayerOnFootState::Initialize()
{

}

void PlayerOnFootState::Update()
{
	// 拾う・落とす入力
	if (player_->GetCommand()->InteractCommand()) {
		player_->GetPickUpManager()->InteractParts();
	}

	// 乗り入力
	if (player_->GetCommand()->ActionCommand() && !player_->GetPickUpManager()->IsPartsHold()) {
		// コアなければ
		if (!player_->GetCore()) {
			return;
		}


		player_->GetStateMachine()->ChangeRequest(IPlayerState::kRideAction);
	}
	// 乗ってなければの処理
	if (!player_->GetWorldTransformAdress()->parent_) {
		if (player_->GetWorldTransformAdress()->GetWorldPosition().y <= 0) {
			player_->GetWorldTransformAdress()->transform_.translate.y = 0.0f;
		}
		else {
			player_->GetWorldTransformAdress()->transform_.translate += Gravity::Execute();
		}
	}

	// 移動用の回転処理
	player_->GetCommand()->RotateCommand();
	player_->GetCommand()->VectorRotate();
	// 移動処理
	const float walkSpeedFactor = GlobalVariables::GetInstance()->GetFloatValue("Player", "WalkSpeed");
	Vector3 velocityDirection = player_->GetCommand()->GetDirect() * walkSpeedFactor;
	player_->GetWorldTransformAdress()->transform_.translate += velocityDirection * kDeltaTime_;

}

void PlayerOnFootState::Exit()
{

}
