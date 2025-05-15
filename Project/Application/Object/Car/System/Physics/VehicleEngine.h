#pragma once
#include "../../../Utility/Timer/FrameTimer.h"

class VehicleEngine
{
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:


	// 加速入力のカウント
	FrameTimer accelInputCounter_;
};
