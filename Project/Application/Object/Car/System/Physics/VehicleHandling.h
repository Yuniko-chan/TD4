#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include <cstdint>

class VehicleHandling
{
public:
	void HandleInput(const float inputX);
	void Update();
	// ステアの向き
	Vector3 GetSteerDirect() { return steerDirection_; }
private:
	// 右
	int8_t rightInputCounter_ = 0;
	bool isRight_ = false;
	// 左
	int8_t leftInputCounter_ = 0;
	bool isLeft_ = false;
	// ステア方向
	Vector3 steerDirection_ = {};
	
};
