#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/Camera/BaseCamera.h"
#include "../Object/Utility/Timer/FrameTimer.h"

/// <summary>
/// 俯瞰カメラ
/// </summary>
class OverheadCamera :
	public BaseCamera
{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(float elapsedTime = 0.0f) override;

	/// <summary>
	/// 回転行列の取得
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetRotateMatrix();

private: // メンバ関数

};
