#include "Obstacle.h"
#include "../../../Collider/CollisionConfig.h"

Obstacle::Obstacle()
{
}

Obstacle::~Obstacle()
{
}

void Obstacle::Initialize(LevelData::MeshData* data)
{

	// 初期化
	MeshObject::Initialize(data);

}

void Obstacle::Initialize(LevelData::MeshData* data, const ObstacleData obstacleData)
{

	// 障害物
    obstacleData;

	// 初期化
	MeshObject::Initialize(data);

}

void Obstacle::Update()
{
}

void Obstacle::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}

void Obstacle::CollisionListRegister(BaseCollisionManager* collisionManager)
{
	MeshObject::CollisionListRegister(collisionManager);
}

void Obstacle::CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{
	MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);
}

void Obstacle::ColliderInitialize(ColliderShape collider)
{

    // 衝突マスク
    collisionAttribute_ = kColisionAttributeGimmick;
    collisionMask_ -= kColisionAttributeGimmick;

    // コライダー
    OBB obb = std::get<OBB>(collider);
    obb.SetParentObject(this);
    obb.SetCollisionAttribute(collisionAttribute_);
    obb.SetCollisionMask(collisionMask_);
    ColliderShape* colliderShape = new ColliderShape();
    *colliderShape = obb;
    collider_.reset(colliderShape);

}
