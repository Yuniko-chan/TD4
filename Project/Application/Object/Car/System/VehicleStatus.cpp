#include "VehicleStatus.h"
#include "../CarLists.h"
#include "../../../Engine/2D/ImguiManager.h"

void VehicleStatus::ImGuiDraw()
{
	ImGui::DragFloat("Armor", &this->armor_);
	if (ImGui::Button("AddArmor")) {
		armor_++;
	}
	ImGui::DragFloat("Speed", &this->speed_);
	if (ImGui::Button("AddSpeed")) {
		speed_++;
	}
	ImGui::DragFloat("Weight", &this->weight_);
	if (ImGui::Button("AddWeight")) {
		weight_++;
	}
}
