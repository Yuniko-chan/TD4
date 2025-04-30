#include "PlayerDebugData.h"
#include "../../../Engine/2D/ImguiManager.h"

PlayerDebugData::RideActionData PlayerDebugData::sRideActionData;
PlayerDebugData::RideActionData PlayerDebugData::sDropoffActionData;
PlayerDebugData::MoveParamData PlayerDebugData::sMoveData;

void PlayerDebugData::ImGuiDraw()
{
	static float sDragValue = 0.1f;
	ImGui::DragFloat("DragValue", &sDragValue, 0.1f);

	sRideActionData.ImGuiDraw("Ride", sDragValue);
	sDropoffActionData.ImGuiDraw("DropOff", sDragValue);
	sMoveData.ImGuiDraw("Move", sDragValue);
}

void PlayerDebugData::RideActionData::ImGuiDraw(const std::string& name, float dragValue)
{
	std::string tagName = name + "JumpHeight";
	ImGui::DragFloat(tagName.c_str(), &jumpHeight, dragValue);
	tagName = name + "ActionFrame";
	ImGui::DragFloat(tagName.c_str(), &actionFrame, dragValue);
	tagName = name + "Offset";
	ImGui::DragFloat3(tagName.c_str(), &offset.x, dragValue);
}

void PlayerDebugData::MoveParamData::ImGuiDraw(const std::string& name, float dragValue)
{
	std::string tagName = name + "MoveSpeed";
	ImGui::DragFloat(tagName.c_str(), &this->moveSpeed, dragValue);
	tagName = name + "RideSpeed";
	ImGui::DragFloat(tagName.c_str(), &this->rideSpeed, dragValue);
}