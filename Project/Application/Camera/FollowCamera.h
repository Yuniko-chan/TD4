#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/Camera/BaseCamera.h"
#include "../Object/Utility/Timer/FrameTimer.h"
#include "Transition/TransitionCameraModule.h"

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera :
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

	/// <summary>
	/// 引きカメラのオフセット追加値
	/// </summary>
	/// <param name="offset"></param>
	void SetZoomOutOffset(const float& offset) { zoomOutOffset_ = offset; }

private: // メンバ関数

	/// <summary>
	/// 追従対象からのオフセットを計算する
	/// </summary>
	/// <returns></returns>
	Vector3 OffsetCalc();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

private:
	void TransitionUpdate() override;

private: // メンバ変数

	//追従対象
	const WorldTransform* target_ = nullptr;

	// ターゲット位置
	Vector3 interTarget_;

	float offsetMoveRate_ = 0.1f;

	// 車両に乗っている状態の
	Vector3 inVehicleRotation_ = {};
	Vector3 inVehicleOffset_ = {};

	// 降りてる時の
	Vector3 onFootRotation_ = {};
	Vector3 onFootOffset_ = {};

	// 方向ベクトル
	bool usedDirection_ = false;
	Vector3 rotateDirection_ = Vector3(0.0f, 0.0f, 1.0f);

	// オフセット
	Vector3 offset_ = {};
	float zoomOutOffset_ = 0.0f;

};

