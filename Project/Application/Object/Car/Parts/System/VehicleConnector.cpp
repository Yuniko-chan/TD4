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
	std::string name;
	if (!parents_.empty()) {
		ImGui::SeparatorText("Parents");
		for (auto it = parents_.begin(); it != parents_.end(); ++it) {
			name = (*it)->GetName();
			ImGui::Text(name.c_str());
			ImGui::Text("\n");
		}
	}
	if (!childrens_.empty()) {
		ImGui::SeparatorText("Childrens");
		for (auto it = childrens_.begin(); it != childrens_.end(); ++it) {
			name = (*it)->GetName();
			ImGui::Text(name.c_str());
			ImGui::Text("\n");
		}
	}
	name = owner_->GetName() + "Data";
	if (ImGui::TreeNode(name.c_str())) {
		bool isDelete = owner_->GetIsDelete();
		ImGui::Checkbox("IsDelete", &isDelete);
		ImGui::DragFloat2("Key", &debugKey_.x);
		ImGui::InputInt("Depth", &depth_);

		ImGui::TreePop();
	}
}

void VehicleConnector::ReleaseParent(Car::IParts* part)
{
	parents_.remove(part);
	// 親がなければ
	if (parents_.empty()) {
		// 子がいれば（その子の親に併せた処理
		if (!childrens_.empty()) {
			int32_t minDepth = 100;	// 100 = エラーコード
			Car::IParts* nextParts = nullptr;
			//---親が居なくなったため新しい繋ぎ先の検索処理---//
			// 子から（自分以外の親がいる子の中で一番小さい深度値の子を）検索
			for (std::list<Car::IParts*>::iterator it = childrens_.begin();
				it != childrens_.end(); ++it) {
				(*it)->GetConnector()->DeleteParent(owner_);
				// 自分を外した上で親が残っているか？
				if ((*it)->GetConnector()->IsParent()) {
					minDepth = (*it)->GetConnector()->GetDepth();
					nextParts = (*it);
				}
			}
			// バグ回避
			if (nextParts) {
				depth_ = minDepth++;
				parents_.push_back(nextParts);
				DeleteChildren(nextParts);
				// 親として登録されていたのを子に切り替える
				nextParts->GetConnector()->DeleteParent(owner_);
				// 子として登録
				nextParts->GetConnector()->AddChildren(owner_);
			}
			else {
				owner_->ReleaseParent();
			}
		}
		// 子がいない為切り替え先がないため解除
		else {
			owner_->ReleaseParent();
		}
	}

}

void VehicleConnector::ReleaseChildren(Car::IParts* part)
{

	childrens_.remove(part);

}
