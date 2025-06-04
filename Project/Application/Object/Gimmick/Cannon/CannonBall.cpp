#include "CannonBall.h"
#include "../../../Collider/CollisionConfig.h"
#include "../../../../Engine/Math/DeltaTime.h"

// 生存時間
const float CannonBall::kLifeTime_ = 1.0f;

// 落下加速度
const float CannonBall::kFallingAcceleration_ = -0.01f;

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
	cannonBallData.speed = 1.0f;

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

	// 爆発初期化
	ExplosionInitialize();

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

		// 生存時間
		lifetimeElapsed_ += kDeltaTime_;
		if (lifetimeElapsed_ >= kLifeTime_) {
			// 動作終了
			Explosion();
		}

	}

	// コライダー更新
	ColliderUpdate();

	cannonExplosion_->Update();

}

void CannonBall::Draw(BaseCamera& camera)
{

	// 動作中表示
	if (isWorking_) {
		MeshObject::Draw(camera);
	}

	// 爆発
	cannonExplosion_->Draw(camera);

}

void CannonBall::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
	// 爆発
	if (isWorking_) {
		Explosion();
	}
}

void CannonBall::CollisionListRegister(BaseCollisionManager* collisionManager)
{

	MeshObject::CollisionListRegister(collisionManager);

	cannonExplosion_->CollisionListRegister(collisionManager);

}

void CannonBall::CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

	MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);

	cannonExplosion_->CollisionListRegister(collisionManager, colliderDebugDraw);

}

void CannonBall::Reset(const CannonBallData& cannonBallData)
{

	// 速度
	velocity_ = Vector3::Multiply(cannonBallData.direction, cannonBallData.speed);
	// 加速度
	acceleration_.y = kFallingAcceleration_;

	// 位置
	worldTransform_.transform_.translate = cannonBallData.position;
	worldTransform_.UpdateMatrix();

	// 動作している
	isWorking_ = true;

	// 生存時間経過
	lifetimeElapsed_ = 0.0f;

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

	// 活動終了なら遠くへ
	if (isWorking_) {
		sphere.center_ = worldTransform_.GetWorldPosition();
	}
	else {
		sphere.center_ = { -10000.0f,-10000.0f ,-10000.0f };
	}


	sphere.radius_ = worldTransform_.transform_.scale.x;

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = sphere;

	collider_.reset(colliderShape);

}

void CannonBall::Explosion()
{

	// 活動終了
	isWorking_ = false;

	// 大砲の弾、リセット
	CannonExplosionData cannonExplosionData;
	cannonExplosionData.explosionRadiusMax = 1.0f;
	cannonExplosionData.explosionTime_ = 1.0f;
	cannonExplosionData.timeToReachRadiusMax = 0.5f;
	cannonExplosionData.position = worldTransform_.GetWorldPosition();

	cannonExplosion_->Reset(cannonExplosionData);

}

void CannonBall::ExplosionInitialize()
{

	cannonExplosion_ = std::make_unique<CannonExplosion>();
	LevelData::MeshData data;
	data.directoryPath = "Resources/default/";
	data.flieName = "ball.obj";
	data.transform = { 1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	data.transform.translate = worldTransform_.GetWorldPosition();
	data.className = "CannonExplosion";
	data.name = "";
	data.parentName = "";
	Sphere collider;
	collider.center_ = { 0.0f,0.0f,0.0f };
	collider.radius_ = 1.0f;
	data.collider = collider;

	CannonExplosionData cannonExplosionData;
	cannonExplosionData.explosionRadiusMax = 1.0f;
	cannonExplosionData.explosionTime_ = 1.0f;
	cannonExplosionData.timeToReachRadiusMax = 0.5f;
	cannonExplosionData.position = worldTransform_.GetWorldPosition();

	cannonExplosion_->Initialize(&data, cannonExplosionData);

}
