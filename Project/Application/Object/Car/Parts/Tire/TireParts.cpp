#include "TireParts.h"
#include "../../../Engine/2D/ImguiManager.h"

void TireParts::Initialize(LevelData::MeshData* data)
{
	Car::IParts::Initialize(data);

	worldTransform_.transform_.translate = Vector3(-2.0f, 0.0f, 0.0f);
}

void TireParts::Update()
{
	Car::IParts::Update();
}

void TireParts::ImGuiDraw()
{
	ImGui::Begin(name_.c_str());
	static float moveValue = 0.1f;
	ImGui::DragFloat("SlideValue", &moveValue, 0.01f);
	// トランスフォーム用
	ImGuiTransform(moveValue);

	ImGui::End();
}
