#include "Cannon.h"
#include "../../../Collider/CollisionConfig.h"
#include "../../../../Engine/Math/DeltaTime.h"

Cannon::Cannon()
{
}

Cannon::~Cannon()
{
}

void Cannon::Initialize(LevelData::MeshData* data)
{

    // 本体初期化
    CannonData cannonData;
    cannonData.cooltimeMax = 3.0f;
    cannonData.rotate = { 0.0f,0.0f,0.0f };
    cannonData.firingDirection = Vector3::Normalize(Vector3{ 0.0f,2.0f,1.0f });
    cannonData.firingSpeed = 0.02f;

    // 初期化
    Initialize(data, cannonData);

}

void Cannon::Initialize(LevelData::MeshData* data, const CannonData& cannonData)
{

    // メッシュオブジェクト初期化
    MeshObject::Initialize(data);

    // 本体初期化
    CannonInitialize(cannonData);

    // 大砲の弾初期化
    CannonBallInitialize();

}

void Cannon::Update()
{

    // 大砲更新
    CannonUpdate();

    // 大砲の弾更新
    cannonBall_->Update();

}

void Cannon::Draw(BaseCamera& camera)
{

	// 本体
	MeshObject::Draw(camera);

    // 大砲の弾
    cannonBall_->Draw(camera);

}

void Cannon::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}

void Cannon::CollisionListRegister(BaseCollisionManager* collisionManager)
{
	
	// 衝突登録
	MeshObject::CollisionListRegister(collisionManager);

    // 大砲の弾
    cannonBall_->CollisionListRegister(collisionManager);

}

void Cannon::CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

	// 衝突登録
	MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);

    // 大砲の弾
    cannonBall_->CollisionListRegister(collisionManager, colliderDebugDraw);

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

    // 回転
    worldTransform_.transform_.rotate = cannonData.rotate;
    worldTransform_.UpdateMatrix();

    // 発射向き
    firingDirection_ = cannonData.firingDirection;

    // 発射速度
    firingSpeed_ = cannonData.firingSpeed;

}

void Cannon::CannonUpdate()
{

    // クールタイム処理
    CooltimeUpdate();

    // 弾発射
    BulletFiring();

}

void Cannon::CooltimeUpdate()
{

    // クールタイム終了か
    if (cooltimeElapsed_ >= cooltimeMax_ && !cannonBall_->GetIsWorking()) {
        // クールタイム初期化
        cooltimeElapsed_ = 0.0f;
        // 発射するか
        isFiring_ = true;
    }
    else {
        if (cooltimeElapsed_ < cooltimeMax_ && !cannonBall_->GetIsWorking()) {
            // クールタイム経過
            cooltimeElapsed_ += kDeltaTime_;
        }
        isFiring_ = false;
    }

}

void Cannon::BulletFiring()
{

    // 発射しない
    if (!isFiring_) {
        return;
    }

    // 大砲の弾、リセット
    CannonBallData data;
    data.direction = firingDirection_;
    data.position = worldTransform_.GetWorldPosition();
    data.speed = firingSpeed_;
    cannonBall_->Reset(data);

}

void Cannon::CannonBallInitialize()
{

    cannonBall_ = std::make_unique<CannonBall>();
    LevelData::MeshData data;
    data.directoryPath = "Resources/default/";
    data.flieName = "ball.obj";
    data.transform = { 1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
    data.transform.translate = worldTransform_.GetWorldPosition();
    data.className = "CannonBall";
    data.name = "";
    data.parentName = "";
    Sphere collider;
    collider.center_ = { 0.0f,0.0f,0.0f };
    collider.radius_ = 1.0f;
    data.collider = collider;

    CannonBallData cannonBallData;
    cannonBallData.direction = { 0.0f,0.0f,1.0f };
    cannonBallData.speed = 0.01f;

    cannonBall_->Initialize(&data, cannonBallData);

}
