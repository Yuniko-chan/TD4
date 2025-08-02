#pragma once
#include "BaseUI.h"
#include "../Object/GameTimer/GameTimeSystem.h"

/// <summary>
/// 時間のUI
/// </summary>
class TimeUI :
    public BaseUI
{

public: // サブクラス

	/// <summary>
	/// 桁
	/// </summary>
	enum DigitIndex
	{
		kDigitIndexOneHundred,
		kDigitIndexTen,
		kDigitIndexOne,
		kDigitIndexOfCount
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
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 桁設定
	/// </summary>
	/// <param name="digitIndex"></param>
	void SetDigitIndex(DigitIndex digitIndex) { digitIndex_ = digitIndex; }

private: // 関数

	/// <summary>
	/// 移動更新
	/// </summary>
	void MoveUpdate();

	/// <summary>
	/// 時間更新
	/// </summary>
	void TimerUpdate();

private: // メンバ変数

	// 桁
	DigitIndex digitIndex_;

	// タイマーシステム
	GameTimeSystem* timerSystem_;

};

