#include "Minigun.h"
#include "../../../Collider/CollisionConfig.h"
#include "../../../../Engine/Math/DeltaTime.h"

// 発射間隔
const float Minigun::kFiringInterval_ = 0.2f;

Minigun::Minigun()
{
}

Minigun::~Minigun()
{
}

void Minigun::Initialize(LevelData::MeshData* data)
{

    // ミニガンデータ
    MinigunData minigunData;
    minigunData.direction = { 0.0f,0.0f,1.0f };
    minigunData.position = { 0.0f,0.0f,0.0f };

    // 初期化
    Initialize(data, minigunData);

}

void Minigun::Initialize(LevelData::MeshData* data, const MinigunData minigunData)
{
    
    // 初期化
    MeshObject::Initialize(data);

    // 経過時間
    elapsedTime_ = 0.0f;

    // 弾の初期化
    bullets_.clear();

    // 位置
    worldTransform_.transform_.translate = minigunData.position;

	// 向き
	worldTransform_.direction_ = minigunData.direction;
    worldTransform_.usedDirection_ = false;

    worldTransform_.UpdateMatrix();

}

void Minigun::Update()
{

    //デスフラグの立った弾を削除
    bullets_.remove_if([](MinigunBullet* bullet) {
        if (bullet->IsDead()) {
            delete bullet;
            return true;
        }
        return false;
        });

    // 時間
    elapsedTime_ += kDeltaTime_;

    // 発射するか
    if (elapsedTime_ >= kFiringInterval_) {
        elapsedTime_ = 0.0f;
    
        // 発射する
        Fire();
    }

    // 弾の更新
    for (MinigunBullet* bullet : bullets_) {
        bullet->Update();
    }

}

void Minigun::Draw(BaseCamera& camera)
{

    // 本体
    MeshObject::Draw(camera);

    // 弾の描画
    for (MinigunBullet* bullet : bullets_) {
        bullet->Draw(camera);
    }

}

void Minigun::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
    colliderPartner, collisionData;
}

void Minigun::CollisionListRegister(BaseCollisionManager* collisionManager)
{

    // 本体
    MeshObject::CollisionListRegister(collisionManager);

    // 弾
    for (MinigunBullet* bullet : bullets_) {
        bullet->CollisionListRegister(collisionManager);
    }

}

void Minigun::CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

    // 本体
    MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);

    // 弾
    for (MinigunBullet* bullet : bullets_) {
        bullet->CollisionListRegister(collisionManager, colliderDebugDraw);
    }

}

void Minigun::ColliderInitialize(ColliderShape collider)
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

void Minigun::Fire()
{

    MinigunBullet* minigunBullet = new MinigunBullet();

    LevelData::MeshData data;
    data.directoryPath = "Resources/Model/Gimmick/IronBall/";
    data.flieName = "IronBall.obj";
    data.transform = { 1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
    data.className = "MinigunBullet";
    data.name = "";
    data.parentName = "";
    Sphere collider;
    collider.center_ = { 0.0f,0.0f,0.0f };
    collider.radius_ = 0.1f;
    data.collider = collider;

    MinigunBulletData minigunBulletData;
    minigunBulletData.direction = worldTransform_.direction_;
    minigunBulletData.position = worldTransform_.GetWorldPosition();

    // 初期化
    minigunBullet->Initialize(&data, minigunBulletData);

    // 追加
    bullets_.push_back(minigunBullet);

}
