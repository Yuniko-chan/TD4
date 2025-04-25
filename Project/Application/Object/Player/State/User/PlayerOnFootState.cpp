#include "PlayerOnFootState.h"
#include "../../Player.h"

#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"

void PlayerOnFootState::Initialize()
{

}

void PlayerOnFootState::Update()
{
	// 切り替え
	if (player_->GetCommand()->ActionCommand()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kRideAction);
	}
	if (!player_->GetWorldTransformAdress()->parent_) {
		if (player_->GetWorldTransformAdress()->GetWorldPosition().y <= 0) {
			player_->GetWorldTransformAdress()->transform_.translate.y = 0.0f;
		}
		else {
			player_->GetWorldTransformAdress()->transform_.translate += Gravity::Execute();
		}
	}

	// 移動処理
	player_->GetWorldTransformAdress()->transform_.translate += player_->GetCommand()->GetDirect() * kDeltaTime_;

}

void PlayerOnFootState::Exit()
{

}
