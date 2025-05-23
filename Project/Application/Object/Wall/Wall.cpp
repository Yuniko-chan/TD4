#include "Wall.h"
#include "../../Collider/CollisionConfig.h"
#include "../../../Engine/Math/DeltaTime.h"

Wall::Wall()
{
}

Wall::~Wall()
{
}

void Wall::Initialize(LevelData::MeshData* data)
{

    
    // 初期化
    MeshObject::Initialize(data);

}

void Wall::Update()
{

}

void Wall::Draw(BaseCamera& camera)
{
    if (isVisible_) {
        MeshObject::Draw(camera);
    }
}

void Wall::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
    colliderPartner, collisionData;
}

void Wall::CollisionListRegister(BaseCollisionManager* collisionManager)
{

    // 衝突登録
    MeshObject::CollisionListRegister(collisionManager);

    
}

void Wall::CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

    // 衝突登録
    MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);


}

void Wall::ColliderInitialize(ColliderShape collider)
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

