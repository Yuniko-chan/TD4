#include "Player.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/2D/ImguiManager.h"

#include "../Car/VehicleCore.h"
#include "State/IPlayerState.h"

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

	playerAnimation_ = std::make_unique<PlayerAnimation>();
	playerAnimation_->Initialize(model_);

	playerCommand_ = std::make_unique<PlayerCommand>();
	playerCommand_->Initialize(this);

	// ステート
	stateMachine_ = std::make_unique<PlayerStateMachine>();
	stateMachine_->Initialize();
	stateMachine_->SetPlayer(this);
}

void Player::Update()
{
	// 基底
	MeshObject::Update();
	
	// アニメーション
	playerAnimation_->Update(0);

	// コマンド処理
	playerCommand_->Update();

	// ステート処理
	stateMachine_->Update();

	// 座標更新
	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

}

void Player::Draw(BaseCamera& camera)
{
	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = playerAnimation_->GetLocalMatrixManager();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);
}

void Player::ImGuiDraw()
{
	ImGui::Begin(className_.c_str());
	// トランスフォーム
	ImGui::DragFloat3("Translate", &worldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("Rotate", &worldTransform_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &worldTransform_.transform_.scale.x, 0.01f);

	if(ImGui::BeginTabBar("System")) {
		// デバッグ用のデータ
		if (ImGui::BeginTabItem("Debug")) {
			debugData_.ImGuiDraw();
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
	colliderPartner, collisionData;
}

void Player::SetParent()
{
	//Vector3 localPosition = pairCore_->GetWorldTransformAdress()->GetWorldPosition() - worldTransform_.GetWorldPosition();
	// 親子設定
	worldTransform_.SetParent(pairCore_->GetWorldTransformAdress());
	worldTransform_.transform_.translate = Vector3(0.0f, 3.0f, 0.0f);
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

WorldTransform* Player::GetCoreTransform()
{
	return pairCore_->GetWorldTransformAdress();
}
