#include "VehicleStatus.h"
#include "../CarLists.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Math/RandomEngine.h"
#include <numeric>

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
		// 左右
		if ((*it).first.x > 0) directionTypes_.right++;
		else if ((*it).first.x < 0) directionTypes_.left++;
		// 前後
		if ((*it).first.y > 0) directionTypes_.forward++;
		else if ((*it).first.y < 0) directionTypes_.backForward++;
	}
}

void VehicleStatus::ImGuiDraw()
{
	static float random = 0.0f;
	if (ImGui::Button("Rand"))
	{
		random = RandomEngine::GetRandom(0, 10);
	}
	ImGui::DragFloat("Randomvalue", &random);
	//int seed = RandomEngine::GetInstance()->GetSeedData();
	//ImGui::InputInt("RandSeed", &seed);
	ImGui::Checkbox("IsOverheat", &isOverheat_);

	ImGui::DragFloat("Armor", &this->armor_);
	ImGui::DragFloat("Speed", &this->speed_);
	ImGui::DragFloat("Weight", &this->weight_);

	// パーツ
	ImGui::SeparatorText("Parts");
	int print = partsTypes_.tire;

	// タイヤ数
	ImGui::InputInt("総タイヤ", &print);
	ImGui::Text("タイヤ X:左 Y:右 Z:前 W:後");
	ImGui::InputFloat4("タイヤ数", &wheelDirectCount_.x);

	if (wheelDirectCount_.x != 0 && wheelDirectCount_.y != 0) {
		//float leftTRight = wheelDirectCount_.x / wheelDirectCount_.y;
		float sum = wheelDirectCount_.x + wheelDirectCount_.y;
		Vector2 tireSide = Vector2(wheelDirectCount_.x / sum, wheelDirectCount_.y / sum);
		ImGui::InputFloat("タイヤ比率:左", &tireSide.y);
		ImGui::InputFloat("タイヤ比率:右", &tireSide.x);
	}


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
		if (key.x > 0) wheelDirectCount_.x++;
		else if (key.x < 0) wheelDirectCount_.y++;
		else if (key.y > 0) wheelDirectCount_.z++;
		else if (key.y < 0) wheelDirectCount_.w++;
	}
	// アーマー
	else if (name == "ArmorFrameParts") partsTypes_.armor++;
	// エンジン
	else if (name == "EngineParts") partsTypes_.engine++;
}

void VehicleStatus::ApplyPartRemove(std::string name, Vector2Int key)
{
	// タイヤ
	if (name == "TireParts") {
		partsTypes_.tire--;
		// 左右
		if (key.x > 0) wheelDirectCount_.x--;
		else if (key.x < 0) wheelDirectCount_.y--;
		// 前後
		else if (key.y > 0) wheelDirectCount_.z--;
		else if (key.y < 0) wheelDirectCount_.w--;
	}
	// アーマー
	else if (name == "ArmorFrameParts") partsTypes_.armor--;
	// エンジン
	else if (name == "EngineParts") partsTypes_.engine--;
}
