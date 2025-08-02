#pragma once
#include "BaseUI.h"

class VehicleCore;

/// <summary>
/// スペックゲージUI
/// </summary>
class SpecGaugeUI :
    public BaseUI
{

public: // サブクラス

	/// <summary>
	/// ゲージ種類
	/// </summary>
	enum GaugeIndex
	{
		kGaugeIndexSpeed,
		kGaugeIndexRightTurn,
		kGaugeIndexLeftTurn,
		kkGaugeIndexOfCount
	};

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
	
	/// <summary>
	/// 種類設定
	/// </summary>
	/// <param name="gaugeIndex"></param>
	void SetGaugeIndex(GaugeIndex gaugeIndex) { gaugeIndex_ = gaugeIndex; }

private: // メンバ変数

	// コア
	VehicleCore* vehicleCore_;
	// 種類
	GaugeIndex gaugeIndex_;
	// スプライトX最大
	float spriteSizeMaxX_;
	// テクスチャX最大
	float textureSizeMaxX_;

};

