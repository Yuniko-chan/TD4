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
	
	playerAnimation_->Update(0);

	playerCommand_->Update();

	stateMachine_->Update();

	// 座標更新
	//worldTransform_.transform_.translate += Gravity::Execute();
	worldTransform_.UpdateMatrix();

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
	if (ImGui::Button("InVehicle")) {
		stateMachine_->ChangeRequest(IPlayerState::kInVehicle);
	}
	if (ImGui::Button("OnFoot")) {
		stateMachine_->ChangeRequest(IPlayerState::kOnFoot);
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
