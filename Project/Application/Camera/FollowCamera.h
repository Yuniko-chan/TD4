#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/Camera/BaseCamera.h"
#include "../Object/Utility/Timer/FrameTimer.h"

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera :
	public BaseCamera
{
public:
	// カメラ画角モード
	enum class AngleMode
	{
		kPlayer,
		kVehicle,
		kEmpty,
	};

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

	/// <summary>
	/// 変更リクエスト
	/// </summary>
	/// <param name="mode"></param>
	void ChangeRequest(AngleMode mode, float frame);

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

	// 遷移用の
	Vector3 startPoint_ = {};
	Vector3 endPoint_ = {};
	Vector3 startDirection_ = {};
	Vector3 endDirection_ = {};

	Vector3 offset_ = {};

	// 状態
	AngleMode mode_ = AngleMode::kVehicle;
	// 切り替えリクエスト
	std::optional<AngleMode> modeRequest_ = std::nullopt;
	// 遷移タイマー
	FrameTimer transitionTimer_;

};

