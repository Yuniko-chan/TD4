#pragma once
#include "../VehicleSystemCommons.h"

#include <algorithm>
#include <optional>
#include <functional>

class VehicleCore;
class VehicleStatus;

// 入力フラグ（前、現在）
using InputFlag = std::pair<bool, bool>;

class DriveHandling : public OwnerComponent<VehicleCore>
{
public:
	/// <summary>
	/// ハンドルの入力
	/// </summary>
	/// <param name="inputX"></param>
	void HandleInput(const float inputX);
	/// <summary>
	/// 前の更新
	/// </summary>
	void PreUpdate();
	/// <summary>
	/// 後の更新ｎ
	/// </summary>
	/// <param name="velocity"></param>
	void PostUpdate(const Vector3& velocity, VehicleStatus* status);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
	// ステアの向き
	Vector3 GetTireDirection() { return tireDirection_; }
	Vector3 GetPreTireDirection() { return preTireDirection_; }
	// 入力があるか
	bool IsInput() { return (isRight_.second || isLeft_.second); }
	bool IsNoneInput() { return (!isRight_.second && !isLeft_.second); }
	// タイヤの割合
	float GetCurrentLeftTire() { return currentLeftTire_; }
	float GetCurrentRightTire() { return currentRightTire_; }
private:
	/// <summary>
	/// 初期化
	/// </summary>
	void Reset();
	// リセットフラグ
	std::optional<std::function<void()>> onReset_ = std::nullopt;

	Vector3 ApplyStatusToHandling(VehicleStatus* status, const float& angle);
	//Vector3 ApplyHandlingToTire();
	void ApplyHandlingToTire();

	/// <summary>
	/// UI用のハンドルの値取得
	/// </summary>
	void GetUIHandlingData(const float& leftTire, const float& rightTire);

private:
	// 入力チェック
	InputFlag isLeft_ = {};
	InputFlag isRight_ = {};
	// 入力カウント
	float inputCounter_ = 0;
	// 受付
	int consecutiveReceptions_ = 0;
	// ステア方向
	Vector3 steerDirection_ = {};

	// タイヤ適応向き
	Vector3 tireDirection_ = {};
	Vector3 preTireDirection_ = {};

	float currentLeftTire_;
	float currentRightTire_;

};
