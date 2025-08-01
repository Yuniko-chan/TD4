#include "ArmorFrameParts.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../../../Engine/GlobalVariables/GlobalVariables.h"

void ArmorFrameParts::Initialize(LevelData::MeshData* data)
{
	Car::IParts::Initialize(data);
	GlobalVariables* global = GlobalVariables::GetInstance();
	hpHandler_.Setup((int16_t)global->GetIntValue("VehiclePartsInfo", "ArmorHP"));

	worldTransform_.transform_.translate = Vector3(0.0f, -2.0f, 0.0f);

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void ArmorFrameParts::Update()
{
	Car::IParts::Update();
}

void ArmorFrameParts::ImGuiDraw()
{

}

void ArmorFrameParts::ImGuiDrawParts()
{
	ImGui::SeparatorText(name_.c_str());
	ImGuiDrawChildParts();
	static float moveValue = 0.1f;
	ImGui::DragFloat("SlideValue", &moveValue, 0.01f);
	// トランスフォーム用
	ImGuiTransform(moveValue);
	float hp = hpHandler_.GetHP();
	ImGui::InputFloat("体力", &hp);
	ImGui::Text("\n");
}
