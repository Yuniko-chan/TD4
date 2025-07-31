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
	/// <summary>
	/// エンジンが壊れた
	/// </summary>
	void OnEngineBroken(const Vector3& direction);
	/// <summary>
	/// ノックバックを適応した座標を出す
	/// </summary>
	/// <returns></returns>
	Vector3 Execute();

private: // 内部関数
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

	void PowerUniform(const Vector3& direction);
	void PowerEaseOut(const Vector3& direction);
	void PowerEaseIn(const Vector3& direction);
private:
	// 壊れた時の処理種類
	enum PowerType
	{
		kUniform,	// 均一
		kEaseIn,	// 徐々に強くなる
		kEaseOut,	// 徐々に弱くなる
	};

	// 壊れた時の計算種類
	PowerType powerType_ = kUniform;

private:
	// スローエフェクト用
	FrameTimer slowTimer_;
	// 受け付けた回数（1フレームごとにリセット
	int acceptCount_ = 0;

	// ノックバックの速度ベクトル
	Vector3 knockback_ = {};
	// 押し出す力
	Vector3 totalPower_ = {};
};
