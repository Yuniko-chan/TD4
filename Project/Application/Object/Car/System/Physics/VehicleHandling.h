#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Utility/Common/OwnerComponent.h"
#include <cstdint>

class VehicleCore;

class VehicleHandling : public OwnerComponent<VehicleCore>
{
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
	// 入力カウント
	int16_t inputCounter_ = 0;
	// 受付
	int16_t consecutiveReceptions_ = 0;
	// ステア方向
	Vector3 steerDirection_ = {};

};
