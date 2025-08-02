#pragma once
#include "BaseUI.h"
#include "../Object/GameTimer/GameTimeSystem.h"
class ClockUI :
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

private: // メンバ変数

	// タイマーシステム
	GameTimeSystem* timerSystem_;

};

