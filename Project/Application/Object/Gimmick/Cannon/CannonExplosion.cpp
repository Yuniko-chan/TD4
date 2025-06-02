#include "CannonExplosion.h"
#include "../../../Collider/CollisionConfig.h"
#include "../../../../Engine/Math/DeltaTime.h"
#include "../../../../Engine/Math/Ease.h"

CannonExplosion::CannonExplosion()
{
}

CannonExplosion::~CannonExplosion()
{
}

void CannonExplosion::Initialize(LevelData::MeshData* data)
{

    CannonExplosionData cannonExplosionData;
    cannonExplosionData.explosionRadiusMax = 1.0f;
    cannonExplosionData.explosionTime_ = 1.0f;
    cannonExplosionData.timeToReachRadiusMax = 0.5f;
    cannonExplosionData.position = { 0.0f,0.0f,0.0f };

    Initialize(data, cannonExplosionData);

}

void CannonExplosion::Initialize(LevelData::MeshData* data, const CannonExplosionData& cannonData)
{

	// メッシュオブジェクト
	MeshObject::Initialize(data);

    // リセット
    Reset(cannonData);

    // 爆発してるか
    isExploding_ = false;

}

void CannonExplosion::Update()
{

    // 時間経過
    elapsedTime_ += kDeltaTime_;

    // 爆発時間チェック
    if (elapsedTime_ >= explosionTime_) {
        // 爆発終了
        isExploding_ = false;
    }

    // 爆発範囲チェック
    float explosionT = std::clamp(elapsedTime_ / timeToReachRadiusMax_, 0.0f, 1.0f);
    explosionRadius_ = Ease::Easing(Ease::EaseName::EaseOutCirc, 0.0f, explosionRadiusMax_, explosionT);

    // 大きさ
    worldTransform_.transform_.scale = { explosionRadius_ , explosionRadius_ , explosionRadius_ };

    worldTransform_.UpdateMatrix();

    // コライダー更新
    ColliderUpdate();

}

void CannonExplosion::Draw(BaseCamera& camera)
{

	// 爆発してるなら描画
	if (isExploding_) {
		MeshObject::Draw(camera);
	}

}

void CannonExplosion::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}

void CannonExplosion::Reset(const CannonExplosionData& cannonExplosionData)
{

    // 爆発してるか
    isExploding_ = true;

    // 爆発時間
    explosionTime_ = cannonExplosionData.explosionTime_;

    // 経過時間
    elapsedTime_ = 0.0f;

    // 爆発半径
    explosionRadius_ = 0.0f;
    
    // 爆発最大半径
    explosionRadiusMax_ = cannonExplosionData.explosionRadiusMax;

    // 半径が最大になるまでの時間
    timeToReachRadiusMax_ = cannonExplosionData.timeToReachRadiusMax;

    // 位置
    worldTransform_.transform_.translate = cannonExplosionData.position;

    // 大きさ
    worldTransform_.transform_.scale = { explosionRadius_ , explosionRadius_ , explosionRadius_ };

}

void CannonExplosion::ColliderInitialize(ColliderShape collider)
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

void CannonExplosion::ColliderUpdate()
{

    Sphere sphere = std::get<Sphere>(*collider_.get());

    sphere.center_ = worldTransform_.GetWorldPosition();

    sphere.radius_ = explosionRadius_;

    ColliderShape* colliderShape = new ColliderShape();

    *colliderShape = sphere;

    collider_.reset(colliderShape);

}
