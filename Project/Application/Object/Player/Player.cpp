#include "Player.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/2D/ImguiManager.h"

#include "../../Collider/CollisionConfig.h"
#include "State/IPlayerState.h"
#include "../Car/VehicleCore.h"
#include "../Car/Parts/Engine/EngineParts.h"
#include "../Car/Parts/Frame/ArmorFrameParts.h"
#include "../Car/Parts/Tire/TireParts.h"

#include "../../../Engine/Collision/Extrusion.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(LevelData::MeshData* data)
{
	// 基底
	MeshObject::Initialize(data);
	material_->SetEnableLighting(HalfLambert);

	// 衝突マスク
	collisionAttribute_ = kCollisionAttributePlayer_;
	collisionMask_ -= kCollisionAttributePlayer_;

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);
	worldTransform_.usedDirection_ = true;
	// システム
	SystemInitialize();
}

void Player::Update()
{
	// 基底
	MeshObject::Update();

	// システム
	SystemUpdate();

	worldTransform_.direction_ = Vector3::Normalize(worldTransform_.direction_);
	// 座標更新
	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

}

void Player::Draw(BaseCamera& camera)
{
	if (!stateMachine_->GetCurrentState()) {
		return;
	}
	const std::type_info& t = typeid(*(stateMachine_->GetCurrentState()));
	std::string name = t.name();
	if (!(name == "class PlayerInVehicleState")) {
		ModelDraw::AnimObjectDesc desc;
		desc.camera = &camera;
		desc.localMatrixManager = playerAnimation_->GetLocalMatrixManager();
		desc.material = material_.get();
		desc.model = model_;
		desc.worldTransform = &worldTransform_;
		ModelDraw::AnimObjectDraw(desc);
	}
}

void Player::ImGuiDraw()
{
	ImGui::Begin(className_.c_str());
	// トランスフォーム
	ImGui::DragFloat3("Translate", &worldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("Rotate", &worldTransform_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &worldTransform_.transform_.scale.x, 0.01f);
	// べく
	ImGui::Checkbox("IsDirect", &worldTransform_.usedDirection_);
	ImGui::DragFloat3("Front", &worldTransform_.direction_.x);

	if(ImGui::BeginTabBar("System")) {
		// デバッグ用のデータ
		if (ImGui::BeginTabItem("Debug")) {
			debugData_.ImGuiDraw();
			ImGui::EndTabItem();
		}
		//
		if (ImGui::BeginTabItem("PickUp")) {
			pickUpManager_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		// ステート
		if (ImGui::BeginTabItem("StateButton")) {
			if (ImGui::Button("InVehicle")) {
				stateMachine_->ChangeRequest(IPlayerState::kInVehicle);
			}
			if (ImGui::Button("OnFoot")) {
				stateMachine_->ChangeRequest(IPlayerState::kOnFoot);
			}
			if (ImGui::Button("Ride")) {
				stateMachine_->ChangeRequest(IPlayerState::kRideAction);
			}
			if (ImGui::Button("DropOff")) {
				stateMachine_->ChangeRequest(IPlayerState::kDropOffAction);
			}
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void Player::ParticleDraw(BaseCamera& camera)
{
	camera;
}

void Player::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	collisionData;
	OBB pushedOut = std::get<OBB>(*collider_);
	OBB pushOut = {};
	Car::IParts* part = nullptr;

	if (std::holds_alternative<VehicleCore*>(colliderPartner)) {
		part = std::get<VehicleCore*>(colliderPartner);
		pushOut = std::get<OBB>(*part->GetCollider());
	} 
	else if (std::holds_alternative<TireParts*>(colliderPartner)) {
		part = std::get<TireParts*>(colliderPartner);
		pushOut = std::get<OBB>(*part->GetCollider());
	}
	else if (std::holds_alternative<ArmorFrameParts*>(colliderPartner)) {
		part = std::get<ArmorFrameParts*>(colliderPartner);
		pushOut = std::get<OBB>(*part->GetCollider());
	}
	else if (std::holds_alternative<EngineParts*>(colliderPartner)) {
		part = std::get<EngineParts*>(colliderPartner);
		pushOut = std::get<OBB>(*part->GetCollider());
	}
	else {
		return;
	}

	// 持ってるパーツの場合はreturn
	if (pickUpManager_->GetHoldParts() == part) {
		return;
	}

	Vector3 extrusion = Extrusion::OBBAndOBB(&pushedOut, &pushOut);
	worldTransform_.transform_.translate += extrusion;
	worldTransform_.UpdateMatrix();

}

void Player::SettingParent()
{
	//Vector3 localPosition = pairCore_->GetWorldTransformAdress()->GetWorldPosition() - worldTransform_.GetWorldPosition();
	// 親子設定
	worldTransform_.SetParent(pairCore_->GetWorldTransformAdress());
	const Vector3 offset = GlobalVariables::GetInstance()->GetVector3Value("Player", "RideOffset");
	worldTransform_.transform_.translate = offset;
}

void Player::ColliderUpdate()
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

void Player::SystemInitialize()
{
	playerAnimation_ = std::make_unique<PlayerAnimation>();
	playerAnimation_->Initialize(model_);

	playerCommand_ = std::make_unique<PlayerCommand>();
	playerCommand_->Initialize(this);

	// ステート
	stateMachine_ = std::make_unique<PlayerStateMachine>();
	stateMachine_->SetOwner(this);
	stateMachine_->Initialize();

	// ピックアップ
	pickUpManager_ = std::make_unique<PlayerPickupManager>();
	pickUpManager_->SetOwner(this);
	pickUpManager_->Initialize();

	// 
	frontChecker_.SetOwner(this);

}

void Player::SystemUpdate()
{
	// 前方管理
	frontChecker_.Update();

	// 拾うマネージャ
	pickUpManager_->Update();

	// アニメーション
	playerAnimation_->Update(0);

	// コマンド処理
	playerCommand_->Update();

	// ステート処理
	stateMachine_->Update();
	
}

WorldTransform* Player::GetCoreTransform()
{
	return pairCore_->GetWorldTransformAdress();
}
