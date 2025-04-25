#include "PlayerRideActionState.h"
#include "../../Player.h"
#include "../../DebugData/PlayerDebugData.h"
#include "../../../Utility/Calc/MotionHelper.h"

void PlayerRideActionState::Initialize()
{
	startPoint_ = player_->GetWorldTransformAdress()->GetWorldPosition();

	animTimer_.Start(PlayerDebugData::sRideActionData.actionFrame);
}

void PlayerRideActionState::Update()
{
	animTimer_.Update();
	if (animTimer_.IsActive()) {
		player_->GetWorldTransformAdress()->transform_.translate = MotionHelper::CurveBezier(startPoint_,
			player_->GetCoreTransform()->GetWorldPosition() + PlayerDebugData::sRideActionData.offset,
			animTimer_.GetElapsedFrame(), PlayerDebugData::sRideActionData.jumpHeight);
	}
	if (animTimer_.IsEnd()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kInVehicle);
	}

}

void PlayerRideActionState::Exit()
{
}
