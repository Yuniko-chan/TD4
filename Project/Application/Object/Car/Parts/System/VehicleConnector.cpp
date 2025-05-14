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

void VehicleConnector::ReleaseParent(Car::IParts* part)
{
	parents_.remove(part);
	// 親がなければ
	if (parents_.empty()) {
		// 子がいれば
		if (!childrens_.empty()) {
			float maxDepth = 0;
			Car::IParts* nextParts = nullptr;
			// 子から検索
			for (std::list<Car::IParts*>::iterator it = childrens_.begin();
				it != childrens_.end(); ++it) {
				if (maxDepth < (*it)->GetConnector()->GetDepth()) {
					maxDepth = (*it)->GetConnector()->GetDepth();
					nextParts = (*it);
				}
			}

			if (depth_ < maxDepth) {
				
			}

		}
	}

}

void VehicleConnector::ReleaseChildren(Car::IParts* part)
{

	childrens_.remove(part);

}
