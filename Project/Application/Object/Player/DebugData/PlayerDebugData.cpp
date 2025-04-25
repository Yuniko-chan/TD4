#include "PlayerDebugData.h"
#include "../../../Engine/2D/ImguiManager.h"

PlayerDebugData::RideActionData PlayerDebugData::sRideActionData;

void PlayerDebugData::ImGuiDraw()
{
	static float sDragValue = 0.1f;
	ImGui::DragFloat("DragValue", &sDragValue, 0.1f);
	ImGui::DragFloat("JumpHeight", &sRideActionData.jumpHeight, sDragValue);
	ImGui::DragFloat("ActionFrame", &sRideActionData.actionFrame, sDragValue);
	ImGui::DragFloat3("RideOffset", &sRideActionData.offset.x, sDragValue);

}
