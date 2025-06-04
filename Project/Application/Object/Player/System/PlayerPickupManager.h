#pragma once
#include "../../../Engine/Collision/BaseCollisionManager.h"
#include "../../Utility/Common/OwnerComponent.h"
#include "../../Utility/Timer/FrameTimer.h"

#include <optional>

// パーツの基底（全ての基）
namespace Car
{
	class IParts;
}

class VehiclePartsManager;
class PickupPointManager;
class Player;

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

class PlayerPickupManager : public OwnerComponent<Player>
{
public:
	PlayerPickupManager();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// せったー
	/// </summary>
	/// <param name="manager"></param>
	void SetPartsManager(VehiclePartsManager* manager) { partsManager_ = manager; }
	void SetPickupPointManager(PickupPointManager* manager) { pickupPointManager_ = manager; }

	/// <summary>
	/// パーツに触れる処理
	/// </summary>
	void InteractParts();

	/// <summary>
	/// パーツを保持しているか
	/// </summary>
	/// <returns></returns>
	bool IsPartsHold() { return holdParts_ ? true : false; }

private:
	/// <summary>
	/// 手放すアクション
	/// </summary>
	void ReleaseAction();
	/// <summary>
	/// 拾うアクション
	/// </summary>
	void CatchAction();

	void PickUp(Car::IParts* parts);

	/// <summary>
	/// 拾うのに成功処理
	/// </summary>
	/// <param name="parts"></param>
	void OnPartCatchSuccess(Car::IParts* parts);
	/// <summary>
	/// 拾うのに失敗処理
	/// </summary>
	void OnCatchFailure();

private:
	// パーツのマネージャ
	VehiclePartsManager* partsManager_ = nullptr;
	// 拾える場所
	PickupPointManager* pickupPointManager_ = nullptr;
	// 持ってるパーツ
	Car::IParts* holdParts_;
	// 拾う用のコライダー
	std::unique_ptr<ColliderShape> collider_;
	// 近いパーツの名前
	std::string nearPartsName_ = "None";
	// 間隔タイマー
	std::optional<FrameTimer> interactDuration_;
};