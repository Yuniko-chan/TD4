#include "VehicleCore.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Input/Input.h"
#include "../GameObjectsList.h"

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
	// 移動処理
	MoveCommand();
	// 座標更新
	//worldTransform_.transform_.translate += Gravity::Execute();
	worldTransform_.UpdateMatrix();
}

void VehicleCore::ImGuiDraw()
{
	ImGui::Begin(className_.c_str());
	if (ImGui::Button("Release")) {
		pairPlayer_ = nullptr;
	}
	ImGui::End();
}

void VehicleCore::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}

void VehicleCore::MoveCommand()
{
	if (!pairPlayer_) {
		return;
	}
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_UPARROW)) {
		worldTransform_.transform_.translate.z += 1.0f;
	}
	else if (input->PushKey(DIK_DOWNARROW)) {
		worldTransform_.transform_.translate.z -= 1.0f;
	}

	if (input->PushKey(DIK_RIGHTARROW)) {
		worldTransform_.transform_.translate.x += 1.0f;
	}
	else if (input->PushKey(DIK_LEFTARROW)) {
		worldTransform_.transform_.translate.x -= 1.0f;
	}
}
