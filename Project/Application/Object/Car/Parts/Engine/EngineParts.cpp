#include "EngineParts.h"
#include "../../../Engine/2D/ImguiManager.h"

void EngineParts::Initialize(LevelData::MeshData* data)
{
	Car::IParts::Initialize(data);

	worldTransform_.transform_.translate = Vector3(0.0f, 2.0f, 0.0f);

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void EngineParts::Update()
{
	Car::IParts::Update();
}

void EngineParts::ImGuiDraw()
{

}

void EngineParts::ImGuiDrawParts()
{
	ImGui::SeparatorText(name_.c_str());
	ImGuiDrawChildParts();
	static float moveValue = 0.1f;
	ImGui::DragFloat("SlideValue", &moveValue, 0.01f);
	// トランスフォーム用
	ImGuiTransform(moveValue);
	ImGui::Text("\n");
}

void EngineParts::OnDetach()
{
	if (hpHandler_.IsDead()) {
		const float kBombPower = 15.0f;
		worldTransform_.transform_.translate.y += kBombPower;
	}
	// 基底処理（座標更新
	IParts::OnDetach();
}
