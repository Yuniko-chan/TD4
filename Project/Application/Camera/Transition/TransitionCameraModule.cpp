#include "TransitionCameraModule.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"

void TransitionCameraModule::Accept(const float& frame)
{
	// 開始
	transitionTimer_.Start(frame);

	//const char* groupName = "FollowCamera";
	//// 開始点
	//from_.first = GlobalVariables::GetInstance()->GetVector3Value(groupName, "inVehicleOffset");
	//from_.second = GlobalVariables::GetInstance()->GetVector3Value(groupName, "inVehicleRotation");
	//// 終着点
	//to_.first = GlobalVariables::GetInstance()->GetVector3Value(groupName, "onFootOffset");
	//to_.second = GlobalVariables::GetInstance()->GetVector3Value(groupName, "onFootRotation");
}

void TransitionCameraModule::TransitionUpdate()
{
	transitionTimer_.Update();
	if (transitionTimer_.IsActive()) {
		this->cameraTransform_->translate =
			Ease::Easing(Ease::EaseName::Lerp, from_.first, to_.first, transitionTimer_.GetElapsedFrame());
		cameraDirection_ = Ease::Easing(Ease::EaseName::Lerp,
			from_.second, to_.second, transitionTimer_.GetElapsedFrame());
	}
}
