#pragma once
#include <cstdint>
#include <array>
#include <functional>
#include <numbers>
#include "../../Engine/Math/Ease.h"
#include "../../Engine/Math/DeltaTime.h"
#include "../../Engine/Math/Vector/Vector2.h"

class OutGameSpriteObject;

/// <summary>
/// アウトゲームのスプライトオブジェクトアニメーション
/// </summary>
class OutGameSpriteObjectAnimation
{

public: // 静的定数

	// アニメーションの番号
	enum AnimationIndex {
		kAnimationIndexNumberRoulette, // 数のルーレット
		kAnimationIndexScaling, // 拡縮
		kAnimationIndexScalingLoop, // 拡縮ループ
		kAnimationIndexMoving, // 移動
		kAnimationIndexMovingLoop, // 移動ループ
		kAnimationIndexOfCount
	};

	// アニメーション関数
	static const std::array<
		std::function<void(OutGameSpriteObject*, OutGameSpriteObjectAnimation*)>,
		kAnimationIndexOfCount> kFunctions_;

public:

	/// <summary>
	/// アニメーション変数
	/// </summary>
	struct AnimationVariable
	{
		float t = 0.0f; // カウント
		Vector2 start = { 0.0f,0.0f }; // 始まり
		Vector2 end = { 0.0f,0.0f }; // 終わり
		Ease::EaseName easeName = Ease::EaseName::Lerp; // イージング方法
		float speed = 0.0f; // スピード
	};

public: // 静的関数

	/// <summary>
	/// 数のルーレット
	/// </summary>
	/// <param name="object">オブジェクト</param>
	/// <param name="animation">アニメーション</param>
	static void NumberRoulette(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation);

	/// <summary>
	/// 拡縮
	/// </summary>
	/// <param name="object">オブジェクト</param>
	/// <param name="animation">アニメーション</param>
	static void Scaling(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation);

	/// <summary>
	/// 拡縮ループ
	/// </summary>
	/// <param name="object">オブジェクト</param>
	/// <param name="animation">アニメーション</param>
	static void ScalingLoop(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation);

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="object">オブジェクト</param>
	/// <param name="animation">アニメーション</param>
	static void Moving(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation);

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="object">オブジェクト</param>
	/// <param name="animation">アニメーション</param>
	static void MovingLoop(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation);

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
	std::array<bool, kAnimationIndexOfCount> doesAnimations_ = {};

	// 拡縮
	AnimationVariable scalingVariable_;
	
	// 移動
	AnimationVariable movingVariable_;

};

