#include "PartsInterface.h"
#include "../VehicleCore.h"
#include "../Manager/VehiclePartsManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"

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
	// 子専用更新（重力の適応）
	ChildUpdate();
	// 一周したときの対策
	const float kOnelap = 6.24f;
	if (worldTransform_.transform_.rotate.y >= kOnelap) {
		worldTransform_.transform_.rotate.y -= kOnelap;
	}
	else if (worldTransform_.transform_.rotate.y <= -kOnelap) {
		worldTransform_.transform_.rotate.y += kOnelap;
	}
	// トランスフォームの更新
	worldTransform_.UpdateMatrix();
	// コライダーの更新
	ColliderUpdate();
}

void Car::IParts::ReleaseParent()
{
	// トランスフォームにおける親子の解除
	Vector3 worldPosition = worldTransform_.GetWorldPosition();
	worldTransform_.SetParent(nullptr);
	worldTransform_.transform_.translate = worldPosition;
	// コアの解除
	parentCore_ = nullptr;
}

void Car::IParts::TransformParent()
{
	// 親コアがあれば
	if (parentCore_) {
		worldTransform_.SetParent(parentCore_->GetWorldTransformAdress());
	}
}

void Car::IParts::SettingParent(VehiclePartsManager* partsManager)
{
	// コアがある場合
	if (parentCore_) {
		worldTransform_.SetParent(parentCore_->GetWorldTransformAdress());
	}
	else {
		// 一番近いコアの取得
		Car::IParts* parts = partsManager->FindNearCoreParts(worldTransform_.GetWorldPosition());
		// ポインタの設定
		parentCore_ = static_cast<VehicleCore*>(parts);
		worldTransform_.SetParent(parentCore_->GetWorldTransformAdress());
		// 子として登録
		parentCore_->AddChild(this);
	}

}

void Car::IParts::ImGuiTransform(const float& value)
{
	// 座標
	std::string name = name_ + ":Translate";
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.translate.x, value);
	// 回転
	name = name_ + ":Rotate";
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.rotate.x, value);
	// スケール
	name = name_ + ":Scale";
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.scale.x, value);

	ImGui::Separator();
	ImGuiDrawChildParts();
}

void Car::IParts::ImGuiDrawChildParts()
{
	if (!parentCore_) {
		return;
	}
	std::string name = name_ + ":Release";
	// 解除
	if (ImGui::Button(name.c_str())) {
		ReleaseParent();
	}
	name = name_ + ":SetUp";
	// 設定
	if (ImGui::Button(name.c_str())) {
		worldTransform_.transform_.translate = {};
		this->TransformParent();
	}
}

void Car::IParts::ColliderUpdate()
{
	// コライダーの更新
	OBB obb = std::get<OBB>(*collider_);
	obb.center_ = worldTransform_.GetWorldPosition();
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);
	// コライダーを設定しなおす
	ColliderShape* shape = new ColliderShape();
	*shape = obb;
	collider_.reset(shape);
}

void Car::IParts::ChildUpdate()
{
	// 親ポインタがある状態
	if (parentCore_) {
		// 親子関係であれば早期
		if (IsParent()) {
			return;
		}
	}
	// 仮の地面処理
	if (worldTransform_.GetWorldPosition().y <= 0.0f) {
		worldTransform_.transform_.translate.y = 0.0f;
		return;
	}
	// 重力
	worldTransform_.transform_.translate += Gravity::Execute();
}

//void Car::IParts::Draw(BaseCamera& camera)
//{
//	MeshObject::Draw(camera);
//}
