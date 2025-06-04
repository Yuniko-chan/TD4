#pragma once
#include "../IPlayerState.h"
#include "../../../Utility/Timer/FrameTimer.h"
#include "../../../Engine/Math/Vector/Vector3.h"

#include <utility>

class PlayerDropOffActionState : public IPlayerState
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
	// アニメーション用のタイマー
	FrameTimer animationTimer_;
	// 1.開始 2.終了
	std::pair<Vector3, Vector3> easePoint_;
};
