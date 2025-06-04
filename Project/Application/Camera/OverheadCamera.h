#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/Camera/BaseCamera.h"
#include "../Object/Utility/Timer/FrameTimer.h"
#include "Transition/TransitionCameraModule.h"

/// <summary>
/// 俯瞰カメラ
/// </summary>
class OverheadCamera :
	public BaseCamera, public TransitionCameraModule
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

	void ImGuiDraw();

	//void RequestSwitch();

private:

	void ApplyGlobalVariable();

	void TransitionUpdate() override;

public: // アクセッサ

	/// <summary>
	/// 追従対象セッター
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target);

	/// <summary>
	/// 回転行列の取得
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetRotateMatrix();

	void SetDirection(const Vector3& direct) { rotateDirection_ = direct; }

private: // メンバ関数

	/// <summary>
	/// 追従対象からのオフセットを計算する
	/// </summary>
	/// <returns></returns>
	Vector3 OffsetCalc();

private: // メンバ関数
	// 追従対象
	const WorldTransform* target_ = nullptr;
	// ターゲット位置
	Vector3 interTarget_;
	// オフセット
	Vector3 offset_ = {};
	// 方向ベクトル
	bool usedDirection_ = false;
	// 回転向きのベクトル
	Vector3 rotateDirection_ = Vector3(0.0f, 0.0f, 1.0f);

	float offsetMoveRate_ = 0.0f;
	Vector3 start_ = {};
};
