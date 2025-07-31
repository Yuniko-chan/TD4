#pragma once
#include "../../System/VehicleSystems.h"

class VehicleCore;
class VehicleStatus;

class KnockbackReactionController : public OwnerComponent<VehicleCore>
{
public:
	/// <summary>
	/// 状態の更新
	/// </summary>
	void Update();

private:
	/// <summary>
	/// 向きの変更開始
	/// </summary>
	void BeginDirectionAdjustment();
	/// <summary>
	///	向きの変更終了
	/// </summary>
	void EndDirectionAdjustment();
	/// <summary>
	/// 向きの変更処理
	/// </summary>
	void UpdateDirectionAdjustment();
public:
	/// <summary>
	/// エンジンが壊れた
	/// </summary>
	void OnEngineBroken(const Vector3& direction);
	Vector3 Execute();
private:
	// 壊れた数（1フレーム内で）
	int breakCount_ = 0;
	// スローエフェクト用
	FrameTimer slowTimer_;

	// 受け付けた回数（1フレームごとにリセット
	int acceptCount_ = 0;

	Vector3 knockback_ = {};
	Vector3 totalPower_ = {};
	Vector3 pushDirection_ = {};
};
