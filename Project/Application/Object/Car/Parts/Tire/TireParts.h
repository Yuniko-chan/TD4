#pragma once
#include "../PartsInterface.h"

class TireParts : public Car::IParts
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;
	/// <summary>
	/// 
	/// </summary>
	void ImGuiDrawParts() override;

	void SetSteerDirection(const Vector3& steer) { steerDirection_ = steer; }
	void SetPreSteerDirection(const Vector3& steer) { preSteerDirection_ = steer; }
	void SetSpinRate(const float rate) { spinRate_ = rate; }
private:
	//
	Vector3 steerDirection_ = { 0.0f,0.0f,1.0f };
	Vector3 preSteerDirection_ = { 0.0f,0.0f,1.0f };
	float spinRate_ = 0.0f;
	float currentSpin_ = 0.0f;
};

