#include "PlayerRideActionState.h"
#include "../../Player.h"
#include "../../DebugData/PlayerDebugData.h"
#include "../../../Utility/Calc/MotionHelper.h"

#include "../../../Engine/Math/Ease.h"

void PlayerRideActionState::Initialize()
{
	startPoint_ = player_->GetWorldTransformAdress()->GetWorldPosition();
	startRotate_ = player_->GetWorldTransformAdress()->transform_.rotate;

	animTimer_.Start(PlayerDebugData::sRideActionData.actionFrame);
}

void PlayerRideActionState::Update()
{
	animTimer_.Update();
	if (animTimer_.IsActive()) {
		player_->GetWorldTransformAdress()->transform_.translate = MotionHelper::CurveBezier(startPoint_,
			player_->GetCoreTransform()->GetWorldPosition() + PlayerDebugData::sRideActionData.offset,
			animTimer_.GetElapsedFrame(), PlayerDebugData::sRideActionData.jumpHeight);

		Vector3 endRotate = player_->GetCoreTransform()->transform_.rotate;
		player_->GetWorldTransformAdress()->transform_.rotate = Ease::Easing(Ease::EaseName::Lerp,
			startRotate_, endRotate, animTimer_.GetElapsedFrame());
	}
	if (animTimer_.IsEnd()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kInVehicle);
	}

}

void PlayerRideActionState::Exit()
{
}
