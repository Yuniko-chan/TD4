#include "TireParts.h"
#include "../../../Engine/2D/ImguiManager.h"

void TireParts::Initialize(LevelData::MeshData* data)
{
	Car::IParts::Initialize(data);

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void TireParts::Update()
{
	Car::IParts::Update();
}

void TireParts::ImGuiDraw()
{

}

void TireParts::ImGuiDrawParts()
{
	ImGui::SeparatorText(name_.c_str());
	ImGuiDrawChildParts();

	static float moveValue = 0.1f;
	ImGui::DragFloat("SlideValue", &moveValue, 0.01f);
	// トランスフォーム用
	ImGuiTransform(moveValue);
	// コネクター
	if (connector_->IsParent()) {
		ImGui::Text("Parent");
	}
	ImGui::Text("\n");
}

void TireParts::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}
