#include "CarCore.h"

CarCore::CarCore()
{
}

CarCore::~CarCore()
{
}

void CarCore::Initialize(LevelData::MeshData* data)
{
	// 基底
	MeshObject::Initialize(data);
	material_->SetEnableLighting(HalfLambert);
	worldTransform_.transform_.translate.z = -300.0f;
}

void CarCore::Update()
{
	// 基底
	MeshObject::Update();

	// 座標更新
	//worldTransform_.transform_.translate += Gravity::Execute();
	worldTransform_.UpdateMatrix();
}

void CarCore::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}
