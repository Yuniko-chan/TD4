#include "VehicleConnector.h"
#include "../PartsInterface.h"

#include "../../../Engine/2D/ImguiManager.h"

void VehicleConnector::Update()
{
	// 親がなければ
	if (parents_.empty()) {
		owner_->SetIsDelete(true);
	}
	else {
		for (std::list<Car::IParts*>::iterator it = parents_.begin(); it != parents_.end();) {
			if ((*it)->GetIsDelete()) {
				it = parents_.erase(it);
			}
			else {
				++it;
			}
		}
	}

}

void VehicleConnector::ImGuiDraw()
{
	if (!parents_.empty()) {
		for (auto it = parents_.begin(); it != parents_.end(); ++it) {
			std::string name = (*it)->GetName();
			ImGui::SeparatorText(name.c_str());
			ImGui::Text("\n");
		}
	}
	bool isDelete = owner_->GetIsDelete();
	ImGui::Checkbox("IsDelete", &isDelete);
	ImGui::DragFloat2("Key", &debugKey_.x);
	ImGui::InputInt("Depth", &depth_);
}
