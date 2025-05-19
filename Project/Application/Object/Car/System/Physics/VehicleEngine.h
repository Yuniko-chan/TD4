#pragma once
#include "../../../Utility/Timer/FrameTimer.h"
#include "../../../KeyConfig/GameKeyconfig.h"
#include "../../../Engine/Math/Vector/Vector3.h"
#include <cstdint>

class VehicleEngine
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

public: // アクセッサ
	//---セッター---//
	void SetDirection(const Vector3& direct) { moveDirect_ = direct; }
	//---ゲッター---//
	Vector3 GetDirection() { return moveDirect_; }
	float GetSpeedRatio() const { return speedRatio_; }
	Vector3 GetAcceleration() { return acceleration_; }

private:
	// 受付連続回数
	int16_t consecutiveReceptions_ = 0;
	// 加速入力
	int32_t accelInputCounter_ = 0;
	bool isAccel_ = false;
	// 減速入力
	int32_t decelInputCounter_ = 0;
	bool isDecel_ = false;

	// 速度
	float speedRatio_ = 0.0f;

	// 移動方向
	Vector3 moveDirect_ = {};
	float controlDirect_ = 0.0f;
	// 加速度
	Vector3 acceleration_ = {};
	// ハンドリングの向き
	Vector3 handringDirect_ = {};
};
