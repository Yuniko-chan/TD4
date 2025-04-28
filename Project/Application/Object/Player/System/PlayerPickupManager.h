#pragma once
#include "../../../Engine/Collision/BaseCollisionManager.h"

// パーツの基底（全ての基）
namespace Car
{
	class IParts;
}

struct PickUpCollision {
	// コライダー
	std::unique_ptr<ColliderShape> collider;
	// トランスフォーム
	WorldTransform worldTransform;
	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0x00000000;
	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};

class PlayerPickupManager
{
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 拾う処理
	/// </summary>
	/// <param name="parts"></param>
	void PickUp(Car::IParts* parts) { holdParts = parts; }

private:
	// 持ってるパーツ
	Car::IParts* holdParts;

	std::unique_ptr<ColliderShape> collider_;
};