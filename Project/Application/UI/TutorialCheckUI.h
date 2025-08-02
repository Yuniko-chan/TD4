#pragma once
#include "BaseUI.h"
#include "../Object/Tutorial/Tutorial.h"

/// <summary>
/// チュートリアルチェックUI
/// </summary>
class TutorialCheckUI :
    public BaseUI
{

public: // サブクラス

	/// <summary>
	/// チュートリアル一覧
	/// </summary>
	enum TutorialIndex
	{
		kTutorialIndexEngine,
		kTutorialIndexTire,
		kTutorialIndexFrame,
		kTutorialIndexOfCount
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
	/// 担当チュートリアル設定
	/// </summary>
	/// <param name="tutorialIndex"></param>
	void SetTutorialIndex(TutorialIndex tutorialIndex) { tutorialIndex_ = tutorialIndex; }

private: // メンバ変数

	// チュートリアル
	Tutorial* tutorial_;

	// 担当チュートリアル
	TutorialIndex tutorialIndex_;

	// うつすか
	bool isDisplay_;

};

