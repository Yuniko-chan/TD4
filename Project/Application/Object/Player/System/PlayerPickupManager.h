#pragma once
#include "../../../Engine/Collision/BaseCollisionManager.h"
#include "PlayerComponent.h"

// パーツの基底（全ての基）
namespace Car
{
	class IParts;
}

class VehiclePartsManager;

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

class PlayerPickupManager : public PlayerComponent
{
public:
	PlayerPickupManager();

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 拾う処理
	/// </summary>
	/// <param name="parts"></param>
	void PickUp(Car::IParts* parts) { holdParts_ = parts; }

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

	/// <summary>
	/// せったー
	/// </summary>
	/// <param name="manager"></param>
	void SetPartsManager(VehiclePartsManager* manager) { partsManager_ = manager; }

	/// <summary>
	/// パーツに触れる処理
	/// </summary>
	void InteractParts();

private:
	// パーツのマネージャ
	VehiclePartsManager* partsManager_ = nullptr;
	// 持ってるパーツ
	Car::IParts* holdParts_;
	// 拾う用のコライダー
	std::unique_ptr<ColliderShape> collider_;
	// 近いパーツの名前
	std::string nearPartsName_ = "None";
};