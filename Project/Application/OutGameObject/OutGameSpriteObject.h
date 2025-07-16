#pragma once
#include <memory>
#include "../../Engine/2D/Sprite.h"
#include "OutGameSpriteObjectAnimation.h"

/// <summary>
/// アウトゲームのスプライトオブジェクト
/// </summary>
class OutGameSpriteObject
{

public: //関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="fileName">ファイルの名前</param>
	/// <param name="position">位置</param>
	/// <param name="color">色</param>
	void Initialize(const std::string& fileName, const Vector2& position, const Vector4& color);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="fileName">ファイルの名前</param>
	/// <param name="position">位置</param>
	/// <param name="color">色</param>
	/// <param name="size">大きさ</param>
	/// <param name="projectionRange">映す範囲</param>
	void Initialize(const std::string& fileName, const Vector2& position, const Vector4& color, const Vector2& size, const Vector2& projectionRange);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// ゲッター
	uint32_t GetTextureHandle() { return textureHandle_; }
	Vector2 GetPosition() { return position_; }
	Vector4 GetColor() { return color_; }
	Vector2 GetSize() { return size_; }
	Vector2 GetProjectionRange() { return projectionRange_; }
	uint32_t GetCurrentSequenceNumber() { return currentSequenceNumber_; }
	uint32_t GetSequentialNumberMax() { return sequentialNumberMax_; }

	OutGameSpriteObjectAnimation* GetAnimation() { return animation_.get(); }
	Sprite* GetSprite() { return sprite_.get(); }

	// セッター
	void SetTextureHandle(uint32_t textureHandle) { textureHandle_ = textureHandle; }
	void SetPosition(const Vector2& position) { position_ = position; }
	void SetColor(const Vector4& color) { color_ = color; }
	void SetSize(const Vector2& size) { size_ = size; }
	void SetProjectionRange(const Vector2& projectionRange) { projectionRange_ = projectionRange; }
	void SetCurrentSequenceNumber(uint32_t currentSequenceNumber) { currentSequenceNumber_ = currentSequenceNumber; }
	void SetSequentialNumberMax(uint32_t sequentialNumberMax) { sequentialNumberMax_ = sequentialNumberMax; }

private: // 変数

	// スプライト
	std::unique_ptr<Sprite> sprite_;
	// テクスチャハンドル
	uint32_t textureHandle_;
	// 位置
	Vector2 position_;
	// 色
	Vector4 color_;
	// サイズ
	Vector2 size_;
	// 映す範囲
	Vector2 projectionRange_;
	// 現在の連番番号
	uint32_t currentSequenceNumber_;
	// 連番数
	uint32_t sequentialNumberMax_;

	// アニメーション	
	std::unique_ptr<OutGameSpriteObjectAnimation> animation_;

};

