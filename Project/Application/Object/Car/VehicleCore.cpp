#include "VehicleCore.h"
#include "../../../Engine/2D/ImguiManager.h"

VehicleCore::VehicleCore()
{
}

VehicleCore::~VehicleCore()
{
}

void VehicleCore::Initialize(LevelData::MeshData* data)
{
	// 基底
	MeshObject::Initialize(data);
	material_->SetEnableLighting(HalfLambert);
	worldTransform_.transform_.translate.z = -300.0f;
}

void VehicleCore::Update()
{
	// 基底
	MeshObject::Update();

	// 座標更新
	//worldTransform_.transform_.translate += Gravity::Execute();
	worldTransform_.UpdateMatrix();
}

void VehicleCore::ImGuiDraw()
{
	ImGui::Begin(className_.c_str());

	ImGui::End();
}

void VehicleCore::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}
