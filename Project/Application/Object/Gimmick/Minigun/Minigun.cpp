#include "Minigun.h"
#include "../../../Collider/CollisionConfig.h"
#include "../../../../Engine/Math/DeltaTime.h"
#include "../../../../Engine/3D/Model/ModelDraw.h"

#include "../../GameTimer/GameTimeSystem.h"

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
    bullet_ = std::make_unique<MinigunBullet>();
    Fire();

	// 向き
	worldTransform_.direction_ = minigunData.direction;
    worldTransform_.usedDirection_ = false;

    worldTransform_.UpdateMatrix();

    // マテリアル
    material_->SetEnableLighting(BlinnPhongReflection);
}

void Minigun::Update()
{

    // 時間
    elapsedTime_ += GameTimeSystem::GetInstance()->GetDeltaTime();
    
    // 弾の更新
    bullet_->Update();

    // 発射するか
    bool fireIt = (elapsedTime_ >= kFiringInterval_) && bullet_->GetIsDead();
    if (fireIt) {
        elapsedTime_ = 0.0f;
        // 発射する
        Fire();
    }

}

void Minigun::Draw(BaseCamera& camera)
{
    // 本体
    MeshObject::Draw(camera);

    // 弾
    bullet_->Draw(camera);

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
    bullet_->CollisionListRegister(collisionManager);

}

void Minigun::CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

    // 本体
    MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);

    // 弾
    bullet_->CollisionListRegister(collisionManager, colliderDebugDraw);

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

    LevelData::MeshData data;
    data.directoryPath = "Resources/Model/Gimmick/IronBall/";
    data.flieName = "IronBall.obj";
    data.transform = { 0.1f,0.1f,0.1f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
    data.className = "MinigunBullet";
    data.name = "";
    data.parentName = "";
    Sphere collider;
    collider.center_ = { 0.0f,0.0f,0.0f };
    collider.radius_ = 0.1f;
    data.collider = collider;

    MinigunBulletData minigunBulletData;
    minigunBulletData.direction = Matrix4x4::TransformNormal(Vector3{ 0.0f,1.0f,0.0f }, worldTransform_.worldMatrix_);
    minigunBulletData.position = worldTransform_.GetWorldPosition();

    // 初期化
    bullet_->Initialize(&data, minigunBulletData);

}
