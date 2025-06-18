#include "ConicalPendulumIronBall.h"

#include <numbers>
#include <cmath>
#include "../../../../Engine/Math/DeltaTime.h"
#include "../../../Collider/CollisionConfig.h"
#include "../../../../Engine/3D/Model/ModelManager.h"
#include "../../../../Engine/3D/Model/ModelDraw.h"

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

    // 紐初期化
    StringInitialize();

}

void ConicalPendulumIronBall::Initialize(LevelData::MeshData* data, const ConicalPendulumIronBallData& conicalPendulumIronBallData)
{

    // メッシュオブジェクト初期化
    MeshObject::Initialize(data);
    
    // 円錐振り子初期化
    ConicalPendulumInitialize(conicalPendulumIronBallData);

    // 紐初期化
    StringInitialize();

}

void ConicalPendulumIronBall::Update()
{

    // メッシュオブジェクト更新
    MeshObject::Update();

    // 円錐振り子更新
    ConicalPendulumUpdate();

    // 行列更新
    worldTransform_.UpdateMatrix();

    // 紐更新
    StringUpdate();

    // コライダー更新
    ColliderUpdate();

}

void ConicalPendulumIronBall::Draw(BaseCamera& camera)
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

void ConicalPendulumIronBall::ColliderUpdate()
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

void ConicalPendulumIronBall::StringInitialize()
{

    // 紐描画
    // ファイル名前
    stringFileName_ = "Rope.obj";
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

void ConicalPendulumIronBall::StringUpdate()
{

    // 向きを更新
    stringWorldTransform_.direction_ = Vector3::Normalize(worldTransform_.GetWorldPosition() - anchor_);
    stringWorldTransform_.UpdateMatrix();

}
