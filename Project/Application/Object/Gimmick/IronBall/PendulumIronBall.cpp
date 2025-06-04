#include "PendulumIronBall.h"

#include <numbers>
#include <cmath>
#include "../../../../Engine/3D/Model/ModelManager.h"
#include "../../../Collider/CollisionConfig.h"
#include "../../../../Engine/3D/Model/ModelDraw.h"
#include "../../../../Engine/Math/DeltaTime.h"

PendulumIronBall::PendulumIronBall()
{
}

PendulumIronBall::~PendulumIronBall()
{
}

void PendulumIronBall::Initialize(LevelData::MeshData* data)
{

    // メッシュオブジェクト初期化
    MeshObject::Initialize(data);

    // 振り子初期化(デバッグ用、最終的には引数でとってくる)
    PendulumIronBallData debugConicalPendulumIronBallData;
    debugConicalPendulumIronBallData.anchor = { 0.0f,10.0f,0.0f };
    debugConicalPendulumIronBallData.angle = 0.7f;
    debugConicalPendulumIronBallData.length = 8.0f;
    PendulumInitialize(debugConicalPendulumIronBallData);

    // 紐初期化
    StringInitialize();

}

void PendulumIronBall::Initialize(LevelData::MeshData* data, const PendulumIronBallData& conicalPendulumIronBallData)
{

    // メッシュオブジェクト初期化
    MeshObject::Initialize(data);

    // 円錐振り子初期化
    PendulumInitialize(conicalPendulumIronBallData);

    // 紐初期化
    StringInitialize();

}

void PendulumIronBall::Update()
{

    // メッシュオブジェクト更新
    MeshObject::Update();

    // 円錐振り子更新
    PendulumUpdate();

    // 行列更新
    worldTransform_.UpdateMatrix();

    // 紐更新
    StringUpdate();

    // コライダー更新
    ColliderUpdate();

}

void PendulumIronBall::Draw(BaseCamera& camera)
{

    MeshObject::Draw(camera);

    if (stringMaterial_->GetMaterialMap()->color.w == 0.0f) {
        return;
    }

    ModelDraw::NormalObjectDesc desc;

    desc.model = stringModel_;
    desc.material = stringMaterial_.get();
    desc.camera = &camera;
    desc.worldTransform = &stringWorldTransform_;

    ModelDraw::NormalObjectDraw(desc);

}

void PendulumIronBall::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
    colliderPartner, collisionData;
}

void PendulumIronBall::ColliderInitialize(ColliderShape collider)
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

void PendulumIronBall::ColliderUpdate()
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

void PendulumIronBall::PendulumInitialize(const PendulumIronBallData& conicalPendulumIronBallData)
{

    // アンカーポイント
    anchor_ = conicalPendulumIronBallData.anchor;
    // 紐の長さ
    length_ = conicalPendulumIronBallData.length;
    //　現在の角度
    angle_ = conicalPendulumIronBallData.angle;
    //角速度
    angularVelocity_ = 0.0f;
    // 角加速度
    angularAcceleration_ = 0.0f;

}

void PendulumIronBall::PendulumUpdate()
{

    // 角加速度を求める
    angularAcceleration_ =
        -(9.8f / length_) * std::sinf(angle_);
    // 各速度を求める
    angularVelocity_ += angularAcceleration_ * kDeltaTime_;
    // 角度を求める
    angle_ += angularVelocity_ * kDeltaTime_;

    // 位置更新
    worldTransform_.transform_.translate.x = anchor_.x + std::sinf(angle_) * length_;
    worldTransform_.transform_.translate.y = anchor_.y - std::cosf(angle_) * length_;
    worldTransform_.transform_.translate.z = anchor_.z;

}

void PendulumIronBall::StringInitialize()
{

    // 紐描画
    // ファイル名前
    stringFileName_ = "IronBallString.obj";
    // モデル
    stringModel_ = ModelManager::GetInstance()->GetModel(directoryPath_, stringFileName_);
    // マテリアル
    stringMaterial_.reset(Material::Create());
    // トランスフォーム
    stringWorldTransform_.Initialize(true);
    stringWorldTransform_.usedDirection_ = true;
    stringWorldTransform_.transform_.translate = anchor_;
    stringWorldTransform_.transform_.scale.z = length_ / 2.0f;
    stringWorldTransform_.UpdateMatrix();

}

void PendulumIronBall::StringUpdate()
{

    // 向きを更新
    stringWorldTransform_.direction_ = Vector3::Normalize(worldTransform_.GetWorldPosition() - anchor_);
    stringWorldTransform_.UpdateMatrix();

}
