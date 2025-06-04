#pragma once
#include "../../BaseCharacter/BaseCharacterAnimation.h"

class VehicleAnimation : public BaseCharacterAnimation
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;

	void Update();
	void AnimationStart();
	//void OpenStart();


	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
private:


};
