#include "TerrainObject.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/2D/ImguiManager.h"

#include "../../Collider/CollisionConfig.h"

void TerrainObject::Initialize(LevelData::MeshData* data)
{
	// 基底
	MeshObject::Initialize(data);
	material_->SetEnableLighting(HalfLambert);

	// 衝突マスク
	collisionAttribute_ = kColisionAttributeTerrain;
	collisionMask_ -= kColisionAttributeTerrain;
	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	worldTransform_.transform_.translate.y = -3.0f;
	worldTransform_.transform_.scale = { 10.0f,1.0f,10.0f };

}

void TerrainObject::Update()
{
	// 基底
	MeshObject::Update();

	// 座標更新
	worldTransform_.UpdateMatrix();

}

void TerrainObject::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}
