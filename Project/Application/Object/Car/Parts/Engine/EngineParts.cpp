#include "EngineParts.h"
#include "../../../Engine/2D/ImguiManager.h"

void EngineParts::Initialize(LevelData::MeshData* data)
{
	Car::IParts::Initialize(data);

	worldTransform_.transform_.translate = Vector3(0.0f, 2.0f, 0.0f);
}

void EngineParts::Update()
{
	Car::IParts::Update();
}

void EngineParts::ImGuiDraw()
{
	ImGui::Begin(name_.c_str());
	static float moveValue = 0.1f;
	ImGui::DragFloat("SlideValue", &moveValue, 0.01f);
	// トランスフォーム用
	ImGuiTransform(moveValue);
	ImGui::DragFloat3("Translate", &worldTransform_.transform_.translate.x, moveValue);
	ImGui::End();
}
