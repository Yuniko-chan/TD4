#pragma once
#include "../VehicleSystemCommons.h"

class VehicleStatus;
class VehicleCore;

class DriveEngine : public OwnerComponent<VehicleCore>
{
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 初期化
	/// </summary>
	void Reset();

	/// <summary>
	/// 受付
	/// </summary>
	void EngineAccept(GameKeyconfig* keyConfig);

	/// <summary>
	/// デバッグ用の
	/// </summary>
	void ImGuiDraw();
private:
	/// <summary>
	/// 速度計算
	/// </summary>
	void SpeedCalculation();

	/// <summary>
	/// 速度出しすぎた時のやつ
	/// </summary>
	/// <param name="SpeedPercentage"></param>
	void OverheatProcess(const float& SpeedPercentage);

	void GetUISpeed();

public: // アクセッサ
	//---ゲッター---//
	float GetAccumulatedAccel_() const { return accumulatedAccel_; }
private:
	// 受付連続回数
	int16_t consecutiveReceptions_ = 0;
	// 加速入力
	bool isAccel_ = false;
	// 減速入力
	bool isDecel_ = false;
	// 入力カウント
	float inputCounter_ = 0;
	// 速度
	float accumulatedAccel_ = 0.0f;

	float maxLimitAccel_ = 0.0f;
	float currentLimitAccel_ = 0.0f;

private:
	// リセットフラグ
	std::optional<std::function<void()>> onReset_ = std::nullopt;

};
