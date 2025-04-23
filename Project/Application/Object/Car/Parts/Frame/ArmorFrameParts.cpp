#include "ArmorFrameParts.h"
#include "../../../Engine/2D/ImguiManager.h"

void ArmorFrameParts::Initialize(LevelData::MeshData* data)
{
	Car::IParts::Initialize(data);

	worldTransform_.transform_.translate = Vector3(0.0f, -2.0f, 0.0f);
}

void ArmorFrameParts::Update()
{
	Car::IParts::Update();
}

void ArmorFrameParts::ImGuiDraw()
{
	ImGui::Begin(name_.c_str());

	ImGui::End();
}
