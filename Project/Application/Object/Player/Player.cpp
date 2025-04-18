#include "Player.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/2D/ImguiManager.h"

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
}

void Player::Update()
{
	// 基底
	MeshObject::Update();
	
	playerAnimation_->Update(0);

	playerCommand_->Update();

	// 座標更新
	//worldTransform_.transform_.translate += Gravity::Execute();
	worldTransform_.UpdateMatrix();

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
