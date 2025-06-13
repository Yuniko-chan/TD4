#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../Utility/Common/OwnerComponent.h"
#include <cstdint>
#include <algorithm>

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
	Vector3 GetSteerDirect() { return steerDirection_; }
	// 入力があるか
	bool IsInput() { return (isRight_.second || isLeft_.second); }

	void SetVehicleDirection(const Vector3& direct) { vehicleDirection_ = direct; }
private:
	// 入力チェック
	InputFlag isLeft_ = {};
	InputFlag isRight_ = {};
	// 入力カウント
	int16_t inputCounter_ = 0;
	// 受付
	int16_t consecutiveReceptions_ = 0;
	// ステア方向
	Vector3 steerDirection_ = {};
	Vector3 executeDirection_ = {};
	// 車体の向き
	Vector3 vehicleDirection_ = {};
};
