#pragma once
#include <memory>
#include "../../Engine/2D/Sprite.h"
#include "../OutGameObject/OutGameSpriteObject.h"

/// <summary>
/// コース踏破システム描画
/// </summary>
class CourseTraversalDraw
{

private: // 定数など

	// テクスチャサイズ
	static const Vector2 kNumberTextureSize_;
	
public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="courseTraversalNum">コース踏破数</param>
	void Initialize(int32_t courseTraversalNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // 変数

	// 踏破数、文字列
	std::unique_ptr<OutGameSpriteObject> traversalNumString_;
	// 踏破数、数10の位
	std::unique_ptr<OutGameSpriteObject> traversalNumTenthPlace_;
	// 踏破数、数1の位
	std::unique_ptr<OutGameSpriteObject> traversalNumOnePlace_;
	// 踏破ランク、文字列
	std::unique_ptr<OutGameSpriteObject> traversalRankString_;
	// 踏破ランク、ランク
	std::unique_ptr<OutGameSpriteObject> raversalRank_;

	// コース踏破数
	int32_t courseTraversalNum_;

};

