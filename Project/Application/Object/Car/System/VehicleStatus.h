#pragma once
#include "../../Utility/Common/OwnerComponent.h"

class VehicleCore;

class VehicleStatus : public OwnerComponent<VehicleCore>
{
public:
	void Update();

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
public:	// アクセッサ
	float GetSpeed() const { return speed_; }
	float GetWeight() const { return weight_; }
	float GetArmor() const { return armor_; }

private:
	float weight_ = 1.0f;
	float speed_ = 1.0f;
	float armor_ = 1.0f;
};
