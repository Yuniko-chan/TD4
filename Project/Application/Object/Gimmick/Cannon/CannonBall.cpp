#include "CannonBall.h"
#include "../../../Collider/CollisionConfig.h"

CannonBall::CannonBall()
{
}

CannonBall::~CannonBall()
{
}

void CannonBall::Initialize(LevelData::MeshData* data)
{

	CannonBallData cannonBallData;
	cannonBallData.direction = { 0.0f,0.0f,1.0f };
	cannonBallData.position = { 0.0f,0.0f,0.0f };
	cannonBallData.speed = 0.02f;

	Initialize(data, cannonBallData);
}

void CannonBall::Initialize(LevelData::MeshData* data, const CannonBallData& cannonBallData)
{

	// メッシュオブジェクト
	MeshObject::Initialize(data);

	// リセット
	Reset(cannonBallData);

	// 初期化では動作させない
	isWorking_ = false;

}

void CannonBall::Update()
{

	// 動作しているか
	if (isWorking_) {
		// 弾速度計算
		velocity_ += acceleration_;

		// 弾移動
		worldTransform_.transform_.translate += velocity_;
		worldTransform_.UpdateMatrix();
	}

}

void CannonBall::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}

void CannonBall::Reset(const CannonBallData& cannonBallData)
{

	// 速度
	velocity_ = { 0.0f,0.0f,0.0f };
	// 加速度
	acceleration_ = Vector3::Multiply(cannonBallData.direction, cannonBallData.speed);
	// 位置
	worldTransform_.transform_.translate = cannonBallData.position;
	worldTransform_.UpdateMatrix();

	// 動作している
	isWorking_ = true;

}

void CannonBall::ColliderInitialize(ColliderShape collider)
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

void CannonBall::ColliderUpdate()
{

	Sphere sphere = std::get<Sphere>(*collider_.get());

	sphere.center_ = worldTransform_.GetWorldPosition();

	sphere.radius_ = worldTransform_.transform_.scale.x;

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = sphere;

	collider_.reset(colliderShape);

}
