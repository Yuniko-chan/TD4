#pragma once
#include "../IPlayerState.h"

class PlayerInVehicleState : public IPlayerState
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 終了
	/// </summary>
	void Exit() override;

private:
	/// <summary>
	/// カメラのオフセット更新
	/// </summary>
	void UpdateCameraOffset();

};
