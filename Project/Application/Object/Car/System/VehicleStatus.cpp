#include "VehicleStatus.h"
#include "../../../Engine/2D/ImguiManager.h"

void VehicleStatus::ImGuiDraw()
{
	ImGui::DragFloat("Armor", &this->armor_);
	ImGui::DragFloat("Speed", &this->speed_);
	ImGui::DragFloat("Weight", &this->weight_);
}
