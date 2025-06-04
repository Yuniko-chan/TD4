#include "VehicleAnimation.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void VehicleAnimation::Initialize(Model* model)
{
	// 基底初期化
	BaseCharacterAnimation::Initialize(model);
	// 初期は止めておく
	animation_.StopAnimation(0);
}

void VehicleAnimation::Update()
{
	// ベースの更新
	BaseCharacterAnimation::Update(0);
	//std::vector<bool> runningData = animation_.FinishedAnimations();
	//if (runningData[0]) {
	//	animation_.StopAnimation(0);
	//}
}

void VehicleAnimation::AnimationStart()
{
	//float frame = GlobalVariables::GetInstance()->GetFloatValue("Player", "RideActionFrame");
	//animation_.AnimationTimerFix(0, 1 / frame);
	animation_.StartAnimation(0, false);
}

void VehicleAnimation::ImGuiDraw()
{
	if (ImGui::Button("Stop"))
	{
		animation_.StopAnimation(0);
	}
	static bool isLoop = false;
	ImGui::Checkbox("IsLoop", &isLoop);
	if (ImGui::Button("Start")) {
		animation_.StartAnimation(0, isLoop);
	}
}
