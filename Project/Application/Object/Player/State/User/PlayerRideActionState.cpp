#include "PlayerRideActionState.h"
#include "../../Player.h"
#include "../../../Car/VehicleCore.h"
#include "../../DebugData/PlayerDebugData.h"
#include "../../../Utility/Calc/MotionHelper.h"

#include "../../../Engine/Math/Ease.h"

void PlayerRideActionState::Initialize()
{
	startPoint_ = player_->GetWorldTransformAdress()->GetWorldPosition();
	startRotate_ = player_->GetWorldTransformAdress()->direction_;
	float frame = GlobalVariables::GetInstance()->GetFloatValue("Player", "RideActionFrame");
	animTimer_.Start(frame);

	player_->GetCameraManager()->SetRequest(ActiveCamera::kFollow);
	// アニメーション開始
	player_->GetCore()->GetAnimation()->AnimationStart();
}

void PlayerRideActionState::Update()
{
	animTimer_.Update();
	if (animTimer_.IsActive()) {
		Vector3 offset = GlobalVariables::GetInstance()->GetVector3Value("Player", "RideOffset");
		float height = GlobalVariables::GetInstance()->GetFloatValue("Player", "RideHeight");
		player_->GetWorldTransformAdress()->transform_.translate = MotionHelper::CurveBezier(startPoint_,
			player_->GetCoreTransform()->GetWorldPosition() + offset,
			animTimer_.GetElapsedFrame(), height);

		Vector3 endRotate = player_->GetCoreTransform()->direction_;
		player_->GetWorldTransformAdress()->direction_ = Ease::Easing(Ease::EaseName::Lerp,
			startRotate_, endRotate, animTimer_.GetElapsedFrame());
	}
	if (animTimer_.IsEnd()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kInVehicle);
	}

}

void PlayerRideActionState::Exit()
{
}
