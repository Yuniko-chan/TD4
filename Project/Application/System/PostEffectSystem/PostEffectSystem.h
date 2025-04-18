#pragma once
#include "../../../Engine/PostEffect/PostEffect.h"
#include "../../../Engine/base/WindowSprite/WindowSprite.h"

/// <summary>
/// ポストエフェクトシステム
/// </summary>
class PostEffectSystem
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 実行
	/// </summary>
	void Execute();

public: // アクセッサ

	/// <summary>
	/// レンダーターゲットテクスチャ設定
	/// </summary>
	/// <param name="renderTargetTexture"></param>
	void SetRenderTargetTexture(RenderTargetTexture* renderTargetTexture) { renderTargetTexture_ = renderTargetTexture; }

private: // メンバ変数

	// ポストエフェクト
	PostEffect* postEffect_;

	// DirectXCommon
	DirectXCommon* dxCommon_;

	// レンダーターゲットテクスチャ
	RenderTargetTexture* renderTargetTexture_;

	// ウインドウスプライト
	WindowSprite* windowSprite_;

};

