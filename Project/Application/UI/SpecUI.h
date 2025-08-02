#pragma once
#include "BaseUI.h"

class VehicleCore;

/// <summary>
/// スペックUI
/// </summary>
class SpecUI :
    public BaseUI
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="position">位置</param>
	/// <param name="size">大きさ</param>
	void Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// コア設定
	/// </summary>
	/// <param name="vehicleCore"></param>
	void SetVehicleCore(VehicleCore* vehicleCore) { vehicleCore_ = vehicleCore; }

private: // メンバ変数

	// コア
	VehicleCore* vehicleCore_;

};

