#include "ConicalPendulumIronBall.h"

#include <numbers>
#include <cmath>
#include "../../../../Engine/Math/DeltaTime.h"
#include "../../../Collider/CollisionConfig.h"

ConicalPendulumIronBall::ConicalPendulumIronBall()
{
}

ConicalPendulumIronBall::~ConicalPendulumIronBall()
{
}

void ConicalPendulumIronBall::Initialize(LevelData::MeshData* data)
{

    // メッシュオブジェクト初期化
    MeshObject::Initialize(data);

    // 円錐振り子初期化(デバッグ用、最終的には引数でとってくる)
    ConicalPendulumIronBallData debugConicalPendulumIronBallData;
    debugConicalPendulumIronBallData.anchor = { 0.0f,10.0f,0.0f };
    debugConicalPendulumIronBallData.halfApexAngle = 0.7f;
    debugConicalPendulumIronBallData.length = 8.0f;
    ConicalPendulumInitialize(debugConicalPendulumIronBallData);

}

void ConicalPendulumIronBall::Initialize(LevelData::MeshData* data, const ConicalPendulumIronBallData& conicalPendulumIronBallData)
{

    // メッシュオブジェクト初期化
    MeshObject::Initialize(data);
    
    // 円錐振り子初期化
    ConicalPendulumInitialize(conicalPendulumIronBallData);

}

void ConicalPendulumIronBall::Update()
{

    // メッシュオブジェクト更新
    MeshObject::Update();

    // 円錐振り子更新
    ConicalPendulumUpdate();

    // 行列更新
    worldTransform_.UpdateMatrix();

}

void ConicalPendulumIronBall::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
    colliderPartner, collisionData;
}

void ConicalPendulumIronBall::ColliderInitialize(ColliderShape collider)
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

void ConicalPendulumIronBall::ConicalPendulumInitialize(const ConicalPendulumIronBallData& conicalPendulumIronBallData)
{

    // アンカー
    anchor_ = conicalPendulumIronBallData.anchor;
    // 紐の長さ
    length_ = conicalPendulumIronBallData.length;
    // 円推の頂角の半分
    halfApexAngle_ = conicalPendulumIronBallData.halfApexAngle;
    // 現在の角度
    angle_ = 0.0f;
    // 各速度
    angularVelocity_ = 0.0f;

}

void ConicalPendulumIronBall::ConicalPendulumUpdate()
{

    // 各速度を求める
    angularVelocity_ = std::sqrtf(9.8f / (length_ * std::cosf(halfApexAngle_)));
    // 現在の角度に加算
    angle_ += angularVelocity_ * kDeltaTime_;

    // 半径と高さを求める
    float radius = std::sinf(halfApexAngle_) * length_;
    float height = std::cosf(halfApexAngle_) * length_;
    // 位置更新
    worldTransform_.transform_.translate.x = anchor_.x + std::cosf(angle_) * radius;
    worldTransform_.transform_.translate.y = anchor_.y - height;
    worldTransform_.transform_.translate.z = anchor_.z - std::sinf(angle_) * radius;

}
