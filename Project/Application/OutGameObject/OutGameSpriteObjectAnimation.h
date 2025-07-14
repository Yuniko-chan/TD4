#pragma once
#include <cstdint>
#include <array>
#include <functional>

class OutGameSpriteObject;

/// <summary>
/// アウトゲームのスプライトオブジェクトアニメーション
/// </summary>
class OutGameSpriteObjectAnimation
{

public: // 静的定数

	// アニメーションの数
	static const uint32_t kAnimationNumberMax_ = 1;

	// アニメーション関数
	static const std::array<
		std::function<void(OutGameSpriteObject*)>,
		kAnimationNumberMax_> kFunctions_;

public: // 静的関数

	/// <summary>
	/// 数のルーレット
	/// </summary>
	/// <param name="object">オブジェクト</param>
	static void NumberRoulette(OutGameSpriteObject* object);

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 実行
	/// </summary>
	/// <param name="object">アウトゲームのスプライトオブジェクト</param>
	void Execute(OutGameSpriteObject* object);

public: // 変数

	// アニメーションするか
	std::array<bool, kAnimationNumberMax_> doesAnimations_ = {};

};

