#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../Object/Utility/Timer/FrameTimer.h"

#include <algorithm>


class TransitionCameraModule
{
public:
	/// <summary>
	/// pair<座標,向き>
	/// </summary>
	using Pose = std::pair<Vector3, Vector3>;

	/// <summary>
	/// 受付
	/// </summary>
	/// <param name="frame"></param>
	void Accept(const float& frame);

protected:

	/// <summary>
	/// 更新
	/// </summary>
	virtual void TransitionUpdate();
	// 現在の値
	Pose cameraPose_{};
	// 元
	Pose from_{};
	// 先
	Pose to_{};
	// 遷移時間
	FrameTimer transitionTimer_;

	EulerTransform* cameraTransform_ = nullptr;
	Vector3 cameraDirection_ = Vector3();
};
