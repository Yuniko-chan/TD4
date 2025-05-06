#pragma once
#include "../IPlayerState.h"
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Utility/Timer/FrameTimer.h"

class PlayerRideActionState : public IPlayerState
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 終了
	/// </summary>
	void Exit() override;

private:
	// 開始点
	Vector3 startPoint_ = {};
	Vector3 startRotate_ = {};
	// アニメーション用タイマー
	FrameTimer animTimer_;

};