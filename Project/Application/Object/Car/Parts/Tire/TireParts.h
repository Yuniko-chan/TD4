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

private:
	//


};

