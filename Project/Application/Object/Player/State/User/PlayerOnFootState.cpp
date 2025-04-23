#include "PlayerOnFootState.h"
#include "../../Player.h"
#include "../../../Engine/Math/DeltaTime.h"

void PlayerOnFootState::Initialize()
{

}

void PlayerOnFootState::Update()
{
	// 切り替え
	if (player_->GetCommand()->ActionCommand()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kInVehicle);
	}

	// 移動処理
	player_->GetWorldTransformAdress()->transform_.translate += player_->GetCommand()->GetDirect() * kDeltaTime_;

}

void PlayerOnFootState::Exit()
{

}
