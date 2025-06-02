#include "VehicleStatus.h"
#include "../CarLists.h"
#include "../../../Engine/2D/ImguiManager.h"

void VehicleStatus::Update()
{

}

void VehicleStatus::StatusUpdate(std::map<Vector2Int, Car::IParts*>* lists)
{
	// 0クリア
	directionTypes_ = {};

	// 方向
	for (std::map<Vector2Int, Car::IParts*>::iterator it = lists->begin(); it != lists->end();
		++it) {
		if ((*it).first.x > 0) {
			directionTypes_.right++;
		}
		else if ((*it).first.x < 0) {
			directionTypes_.left++;
		}

		if ((*it).first.y > 0)
		{
			directionTypes_.forward++;
		}
		else if ((*it).first.y < 0) {
			directionTypes_.backForward++;
		}
	}
}

void VehicleStatus::ImGuiDraw()
{
	ImGui::DragFloat("Armor", &this->armor_);
	ImGui::DragFloat("Speed", &this->speed_);
	ImGui::DragFloat("Weight", &this->weight_);

	// パーツ
	ImGui::SeparatorText("Parts");
	int print = partsTypes_.tire;

	// タイヤ数
	ImGui::InputInt("総タイヤ", &print);
	ImGui::InputInt("左タイヤ", &wheelLeft_);
	ImGui::InputInt("右タイヤ", &wheelRight_);
	ImGui::InputInt("前タイヤ", &wheelFront_);
	ImGui::InputInt("後タイヤ", &wheelReal_);


	print = partsTypes_.armor;
	ImGui::InputInt("Armor", &print);
	print = partsTypes_.engine;
	ImGui::InputInt("Engine", &print);

	// それぞれの向き
	ImGui::SeparatorText("Directions");
	print = directionTypes_.left;
	ImGui::InputInt("Left", &print);
	print = directionTypes_.right;
	ImGui::InputInt("Right", &print);
	print = directionTypes_.forward;
	ImGui::InputInt("Forward", &print);
	print = directionTypes_.backForward;
	ImGui::InputInt("BackForward", &print);

}

void VehicleStatus::ApplyPartAdd(std::string name, Vector2Int key)
{
	// パーツの種類チェック
	if (name == "TireParts") {
		partsTypes_.tire++;
		// 左右のチェック
		if (key.x > 0) {
			wheelRight_++;
		}
		else if (key.x < 0) {
			wheelLeft_++;
		}
		else if (key.y > 0) {
			wheelFront_++;
		}
		else if (key.y < 0) {
			wheelReal_++;
		}
	}
	else if (name == "ArmorFrameParts") {
		partsTypes_.armor++;
	}
	else if (name == "EngineParts") {
		partsTypes_.engine++;
	}
}

void VehicleStatus::ApplyPartRemove(std::string name, Vector2Int key)
{
	if (name == "TireParts") {
		partsTypes_.tire--;
		// 左右のチェック
		if (key.x > 0) {
			wheelRight_--;
		}
		else if (key.x < 0) {
			wheelLeft_--;
		}
		else if (key.y > 0) {
			wheelFront_--;
		}
		else if (key.y < 0) {
			wheelReal_--;
		}
	}
	else if (name == "ArmorFrameParts") {
		partsTypes_.armor--;
	}
	else if (name == "EngineParts") {
		partsTypes_.engine--;
	}
}
