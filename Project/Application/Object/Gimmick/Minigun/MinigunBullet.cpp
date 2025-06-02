#include "MinigunBullet.h"
#include "../../../Collider/CollisionConfig.h"
#include "../../../../Engine/Math/DeltaTime.h"

// 生存時間
const float MinigunBullet::kLifeTime_ = 0.5f;
// 速度
const float MinigunBullet::kSpeed_ = 1.0f;

MinigunBullet::MinigunBullet()
{
}

MinigunBullet::~MinigunBullet()
{
}

void MinigunBullet::Initialize(LevelData::MeshData* data)
{

    MinigunBulletData minigunBulletData;
    minigunBulletData.direction = { 0.0f,0.0f,0.0f };
    minigunBulletData.position = { 0.0f,0.0f,0.0f };

    Initialize(data, minigunBulletData);

}

void MinigunBullet::Initialize(LevelData::MeshData* data, const MinigunBulletData minigunBulletData)
{

    // メッシュオブジェクト
    MeshObject::Initialize(data);

    // 経過時間
    elapsedTime_ = 0.0f;

    // 死亡しているか
    isDead_ = false;

    // 速度
    velocity_ = Vector3::Multiply(minigunBulletData.direction, kSpeed_);

    worldTransform_.transform_.translate = minigunBulletData.position;
    worldTransform_.UpdateMatrix();

}

void MinigunBullet::Update()
{

    // 死亡判定
    if (elapsedTime_ >= kLifeTime_) {
        // 死亡
        isDead_ = true;
    }
    else {
        // 経過時間
        elapsedTime_ += kDeltaTime_;
    }

    worldTransform_.transform_.translate += velocity_;
    worldTransform_.UpdateMatrix();

    // コライダー更新
    ColliderUpdate();

}

void MinigunBullet::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}

void MinigunBullet::ColliderInitialize(ColliderShape collider)
{

    // 衝突マスク
    collisionAttribute_ = kColisionAttributeGimmick;
    collisionMask_ -= kColisionAttributeGimmick;

    // コライダー
    Sphere sphere = std::get<Sphere>(collider);
    sphere.SetParentObject(this);
    sphere.SetCollisionAttribute(collisionAttribute_);
    sphere.SetCollisionMask(collisionMask_);
    ColliderShape* colliderShape = new ColliderShape();
    *colliderShape = sphere;
    collider_.reset(colliderShape);

}

void MinigunBullet::ColliderUpdate()
{

    Sphere sphere = std::get<Sphere>(*collider_.get());

    sphere.center_ = worldTransform_.GetWorldPosition();

    // モデルサイズに合わせる
    const float kModelMagnification = 3.5f;

    sphere.radius_ = worldTransform_.transform_.scale.x * kModelMagnification;

    ColliderShape* colliderShape = new ColliderShape();

    *colliderShape = sphere;

    collider_.reset(colliderShape);

}
