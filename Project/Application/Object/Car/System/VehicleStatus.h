#pragma once

class VehicleCore;

class VehicleStatus
{
public:

	void SetCore(VehicleCore* core) { core_ = core; }
	void ImGuiDraw();
private:
	VehicleCore* core_ = nullptr;

	float weight_ = 0.0f;
	float speed_ = 0.0f;
	float armor_ = 0.0f;
};
