#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include <cstdint>

class VehicleHandling
{
private:
	enum Direction {
		kForward,
		kLeft,
		kRight,
	};
public:
	void HandleInput(const float inputX);
	void Update();
	void ImGuiDraw();
	// ステアの向き
	Vector3 GetSteerDirect() { return steerDirection_; }
	// 入力があるか
	bool IsInput() { return (isRight_ || isLeft_); }
private:
	// 右
	bool isRight_ = false;
	// 左
	bool isLeft_ = false;
	int8_t isDirection_ = kForward;
	// 入力カウント
	int16_t inputCounter_ = 0;
	// 受付
	int16_t consecutiveReceptions_ = 0;
	// ステア方向
	Vector3 steerDirection_ = {};
	
};
