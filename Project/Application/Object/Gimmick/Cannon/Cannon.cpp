#include "Cannon.h"
#include "../../../Collider/CollisionConfig.h"

Cannon::Cannon()
{
}

Cannon::~Cannon()
{
}

void Cannon::Initialize(LevelData::MeshData* data)
{

    // メッシュオブジェクト初期化
    MeshObject::Initialize(data);

    // 本体初期化
    CannonData cannonData;
    cannonData.cooltimeMax = 3.0f;
    cannonData.isLockOnSnip = false;
    cannonData.rotationSpeed = 0.1f;
    CannonInitialize(cannonData);


}

void Cannon::Initialize(LevelData::MeshData* data, const CannonData& cannonData)
{

    // メッシュオブジェクト初期化
    MeshObject::Initialize(data);

    // 本体初期化
    CannonInitialize(cannonData);

}

void Cannon::Update()
{



}

void Cannon::Draw(BaseCamera& camera)
{

	// 
	MeshObject::Draw(camera);

}

void Cannon::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}

void Cannon::CollisionListRegister(BaseCollisionManager* collisionManager)
{
	
	// 衝突登録
	MeshObject::CollisionListRegister(collisionManager);

}

void Cannon::CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

	// 衝突登録
	MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);

}

void Cannon::ColliderInitialize(ColliderShape collider)
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

void Cannon::CannonInitialize(const CannonData& cannonData)
{

    // 撃つ状態か
    readyToFire_ = false;

    // クールタイム経過
    cooltimeElapsed_ = 0.0f;

    // クールタイム値
    cooltimeMax_ = cannonData.cooltimeMax;

    // 狙い撃ち状態か
    isLockOnSnip_ = cannonData.isLockOnSnip;

    // 向き
    direction_ = { 0.0f,0.0f,1.0f };

    // 合わせる向き
    directionToMatch_ = { 0.0f,0.0f,1.0f };

    // 回転速度
    rotationSpeed_ = cannonData.rotationSpeed;

}

void Cannon::CannonUpdate()
{

    // クールタイム処理

    // 狙い撃ち処理

    // 弾発射

}
