#include "PartsInterface.h"
#include "../VehicleCore.h"
#include "../../../Engine/2D/ImguiManager.h"

void Car::IParts::Initialize(LevelData::MeshData* data)
{
	// 基底
	MeshObject::Initialize(data);
	material_->SetEnableLighting(HalfLambert);

}

void Car::IParts::Update()
{
	// メッシュの更新
	MeshObject::Update();
	// トランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Car::IParts::ParentSetting(bool isAccept, const Vector3& offset)
{
	if (isAccept && parentCore_) {
		worldTransform_.SetParent(parentCore_->GetWorldTransformAdress());
		worldTransform_.transform_.translate = offset;
	}
}

void Car::IParts::TransformParent()
{
	if (parentCore_) {
		worldTransform_.SetParent(parentCore_->GetWorldTransformAdress());
	}
}

void Car::IParts::ImGuiTransform(const float& value)
{
	// 座標
	std::string name = name_ + "Translate";
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.translate.x, value);
	// 回転
	name = name_ + "Rotate";
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.rotate.x, value);
	// スケール
	name = name_ + "Scale";
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.scale.x, value);
}

//void Car::IParts::Draw(BaseCamera& camera)
//{
//	MeshObject::Draw(camera);
//}
