#include "CourseDemoObject.h"
#include "../../../Engine/Input/Input.h"

void CourseDemoObject::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	material_->SetEnableLighting(HalfLambert);

	// コライダー
	OBB obb;
	obb.center_ = { 0.0f,0.0f,0.0f };
	obb.otientatuons_[0] = { 1.0f,0.0f,0.0f };
	obb.otientatuons_[1] = { 0.0f,1.0f,0.0f };
	obb.otientatuons_[2] = { 0.0f,0.0f,1.0f };
	obb.size_ = { 0.1f,0.1f,0.1f };
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void CourseDemoObject::Update()
{

	if (Input::GetInstance()->PushKey(DIK_0)) {
		worldTransform_.transform_.translate.z += 0.01f;
	}

	MeshObject::Update();

	// コライダー
	OBB obb = std::get<OBB>(*collider_);
	obb.center_ = worldTransform_.GetWorldPosition();
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);
	// サイズ更新あるならここ
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void CourseDemoObject::ImGuiDraw()
{
}
