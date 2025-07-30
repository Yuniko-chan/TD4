#pragma once
#include "../OutGameObject/OutGameSpriteObject.h"

/// <summary>
/// タイトルスプライトオブジェクト
/// </summary>
class TitleSpriteObjects
{

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // 変数

	// タイトルロゴ
	std::unique_ptr<OutGameSpriteObject> titleLogo_;

	// ボタンをおしてください
	std::unique_ptr<OutGameSpriteObject> pleasePressTheButton_;

};

