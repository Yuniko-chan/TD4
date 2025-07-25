#include "TransitionCameraModule.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/2D/ImguiManager.h"

void TransitionCameraModule::Accept(const float& frame)
{
	// 開始
	transitionTimer_.Start(frame);
	currentPose_.first =
		Ease::Easing(Ease::EaseName::Lerp, from_.first, to_.first, transitionTimer_.GetElapsedFrame());
	currentPose_.second = Ease::Easing(Ease::EaseName::Lerp,
		from_.second, to_.second, transitionTimer_.GetElapsedFrame());
}

void TransitionCameraModule::TransitionUpdate()
{
	transitionTimer_.Update();
	if (transitionTimer_.IsActive()) {
		currentPose_.first =
			Ease::Easing(Ease::EaseName::Lerp, from_.first, to_.first, transitionTimer_.GetElapsedFrame());
		currentPose_.second = Ease::Easing(Ease::EaseName::Lerp,
			from_.second, to_.second, transitionTimer_.GetElapsedFrame());
	}
}

void TransitionCameraModule::ImGuiDraw() {
	ImGui::DragFloat3("FromDirect", &from_.second.x);
	ImGui::DragFloat3("ToDirect", &to_.second.x);
}