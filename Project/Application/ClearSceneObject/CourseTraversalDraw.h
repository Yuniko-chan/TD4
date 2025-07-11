#pragma once
#include <memory>
#include "../../Engine/2D/Sprite.h"

/// <summary>
/// コース踏破システム描画
/// </summary>
class CourseTraversalDraw
{

private: // 定数など

	// テクスチャサイズ
	static const Vector2 kNumberTextureSize_;
	
	// スプライトデータ
	struct SpriteData
	{
		std::unique_ptr<Sprite> sprite;
		uint32_t textureHandle;
		Vector2 position;
		Vector4 color;
		Vector2 size;
	};

	// ルーレットデータ
	struct RouletteData
	{
		// 描画のする数字
		uint32_t drawNumber;
		// ルーレットするか
		bool duringRoulette;
	};

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="courseTraversalNum">コース踏破数</param>
	void Initialize(int32_t courseTraversalNum);

	void Update();

	void Draw();

private: // 関数

	/// <summary>
	/// 数のルーレット
	/// </summary>
	void NumberRoulette();

private: // 変数

	// 踏破数、文字列
	SpriteData traversalNumString_;
	// 踏破数、数10の位
	SpriteData traversalNumTenthPlace_;
	// 踏破数、数1の位
	SpriteData traversalNumOnePlace_;
	
	// 踏破数、数10の位ルーレット
	RouletteData traversalNumTenthPlaceRoulette_;
	// 踏破数、数1の位ルーレット
	RouletteData traversalNumOnePlaceRoulette_;

	// 踏破ランク、文字列
	SpriteData traversalRankString_;

	// 踏破ランク、ランク
	SpriteData raversalRank_;
	// 踏破ランク、ランクルーレット
	RouletteData raversalRankRoulette_;

	// コース踏破数
	int32_t courseTraversalNum_;

};

