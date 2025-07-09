#include "PartsInterface.h"
#include "../VehicleCore.h"
#include "../Manager/VehiclePartsManager.h"
#include "../../../Collider/CollisionConfig.h"

#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"
#include "System/PartCollisionHandler.h"

int32_t Car::SerialNumberGenerate::sSerialArmor = 0;
int32_t Car::SerialNumberGenerate::sSerialEngine = 0;
int32_t Car::SerialNumberGenerate::sSerialTire = 0;
int32_t Car::SerialNumberGenerate::sSerialCore = 0;

void Car::IParts::Initialize(LevelData::MeshData* data)
{
	// 基底
	MeshObject::Initialize(data);
	material_->SetEnableLighting(HalfLambert);
	// 方向フラグ
	worldTransform_.usedDirection_ = true;

	// 衝突マスク
	collisionAttribute_ = kCollisionAttributeVehicleParts_;
	collisionMask_ -= kCollisionAttributeVehicleParts_;

	connector_ = std::make_unique<VehicleConnector>();
	connector_->SetOwner(this);

	hpHandler_.SetOwner(this);
	hpHandler_.Initialize();
}

void Car::IParts::Update()
{
	// メッシュの更新
	MeshObject::Update();
	// 子専用更新（重力の適応）
	ChildUpdate();
	// トランスフォームの更新
	worldTransform_.direction_ = Vector3::Normalize(worldTransform_.direction_);
	worldTransform_.UpdateMatrix();
	// コライダーの更新
	ColliderUpdate();

	// HP更新
	hpHandler_.Update();

	//// HPがなくなり次第Deleteフラグをセット
	//if (hitPoint_ <= 0) {
	//	isDelete_ = true;
	//}

}

void Car::IParts::ReleaseParent()
{
	// トランスフォームにおける親子の解除
	Vector3 worldPosition = worldTransform_.GetWorldPosition();
	worldTransform_.SetParent(nullptr);
	worldTransform_.transform_.translate = worldPosition;
	// コアの解除
	parentCore_ = nullptr;
	// コネクターのリセット
	if (connector_) {
		connector_->Reset();
	}
}

void Car::IParts::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	collisionData;
	PartCollisionHandler::Execute(this, colliderPartner);

}

void Car::IParts::TransformParent()
{
	// 親コアがあれば
	if (parentCore_) {
		worldTransform_.SetParent(parentCore_->GetWorldTransformAdress());
	}
}

bool Car::IParts::SettingParent(VehiclePartsManager* partsManager)
{
	// True:親への合体に成功した場合
	// false:適した部分への合体ができなかった場合｜｜例外処理

	// コアがある場合
	if (parentCore_) {
		worldTransform_.SetParent(parentCore_->GetWorldTransformAdress());
		return false;
	}
	else {
		// 一番近いコアの取得
		Car::IParts* parts = partsManager->FindNearCoreParts(worldTransform_.GetWorldPosition());
		// ポインタの設定
		parentCore_ = static_cast<VehicleCore*>(parts);
		// 親に設定
		bool isSuccess = parentCore_->GetConstructionSystem()->Attach(this);
		if (isSuccess) {
			return true;
		}
		parentCore_ = nullptr;
	}

	return false;
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

	name = name_ + ":HP";
	float hp = this->hpHandler_.GetHP();
	ImGui::DragFloat(name.c_str(), &hp, value);
	// 方向
	name = name_ + ":Direction";
	ImGui::DragFloat3(name.c_str(), &worldTransform_.direction_.x, value);
	worldTransform_.direction_ = Vector3::Normalize(worldTransform_.direction_);
	// 方向ベクトルを使用するか
	name = name_ + ":UseDirection";
	ImGui::Checkbox(name.c_str(), &worldTransform_.usedDirection_);

	ImGui::Separator();
	// 接続処理確認
	if (connector_) {
		name = name_ + ":Connector";
		if (ImGui::TreeNode(name.c_str())) {
			connector_->ImGuiDraw();
			ImGui::TreePop();
		}
	}
}

void Car::IParts::ImGuiDrawChildParts()
{
	if (!parentCore_) {
		return;
	}
	std::string name = name_ + ":ChildData";
	name = name_ + ":Release";
	// 解除
	if (ImGui::Button(name.c_str())) {
		// 倒す処理
		hpHandler_.OnHit(20);
	}
	name = name_ + ":ChildData";
	if (ImGui::TreeNode(name.c_str())) {
		name = name_ + ":SetUp";
		// 設定
		if (ImGui::Button(name.c_str())) {
			worldTransform_.transform_.translate = {};
			this->TransformParent();
		}
		ImGui::TreePop();
	}
}

void Car::IParts::ColliderUpdate()
{
	// コライダーの更新
	OBB obb = std::get<OBB>(*collider_);
	obb.center_ = worldTransform_.GetWorldPosition();

	if (parentCore_) {
		obb.SetOtientatuons(Matrix4x4::Multiply(worldTransform_.rotateMatrix_, worldTransform_.parent_->parentMatrix_));
	}
	else {
		obb.SetOtientatuons(worldTransform_.rotateMatrix_);
	}
	obb.size_ = worldTransform_.transform_.scale;
	// コライダーを設定しなおす
	ColliderShape* shape = new ColliderShape();
	*shape = obb;
	collider_.reset(shape);
}

void Car::IParts::ChildUpdate()
{
	// 親があれば
	if (IsParent()) {
		// 親がある場合コネクターの更新を入れる
		connector_->Update();
		return;
	}
	// 仮の地面処理（後で消す）
	if (worldTransform_.GetWorldPosition().y <= 0.0f) {
		worldTransform_.transform_.translate.y = 0.0f;
		return;
	}
	// 重力
	worldTransform_.transform_.translate += Gravity::Execute();
}
