#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../Utility/Common/OwnerComponent.h"
#include <cstdint>

class VehicleCore;

class VehicleHandling : public OwnerComponent<VehicleCore>
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
	void PostUpdate(const Vector3& velocity, float leftWheel, float rightWheel);
	/// <summary>
	/// ImGui
	/// </summary>
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
