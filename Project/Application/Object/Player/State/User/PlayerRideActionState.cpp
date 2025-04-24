#include "PlayerRideActionState.h"
#include "../../Player.h"
#include "../../../Utility/Calc/MotionHelper.h"

void PlayerRideActionState::Initialize()
{

	startPoint_ = player_->GetWorldTransformAdress()->GetWorldPosition();
	float second = 1.0f;
	animTimer_.Start(second * 60.0f);
}

void PlayerRideActionState::Update()
{
	animTimer_.Update();
	if (animTimer_.IsActive()) {
		player_->GetWorldTransformAdress()->transform_.translate = MotionHelper::CurveBezier(startPoint_,
			player_->GetCoreTransform()->GetWorldPosition(), animTimer_.GetElapsedFrame(), 20.0f);
	}
	if (animTimer_.IsEnd()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kInVehicle);
	}

}

void PlayerRideActionState::Exit()
{
}
