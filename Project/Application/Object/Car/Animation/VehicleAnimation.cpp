#include "VehicleAnimation.h"
#include "../../../Engine/2D/ImguiManager.h"

void VehicleAnimation::Initialize(Model* model)
{
	// 基底初期化
	BaseCharacterAnimation::Initialize(model);

}

void VehicleAnimation::ImGuiDraw()
{
	if (ImGui::Button("Stop"))
	{
		animation_.StopAnimation(0);
	}
	if (ImGui::Button("Start")) {
		animation_.StartAnimation(0, true);
	}
}
