#pragma once
#include "../../../Engine/PostEffect/PostEffect.h"
#include "../../../Engine/base/WindowSprite/WindowSprite.h"
#include "../../Object/Car/System/Drive/DriveSystem.h"
#include "../../Object/Player/Player.h"

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

	/// <summary>
	/// ドライブシステム設定
	/// </summary>
	/// <param name="driveSystem"></param>
	void SetDriveSystem(DriveSystem* driveSystem) { driveSystem_ = driveSystem; }

	/// <summary>
	/// プレイヤー設定
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

private:

	/// <summary>
	/// グローバル変数を適用する
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// グローバル変数を登録する
	/// </summary>
	void RegisteringGlobalVariables();

private: // メンバ変数

	// ポストエフェクト
	PostEffect* postEffect_;

	// DirectXCommon
	DirectXCommon* dxCommon_;

	// レンダーターゲットテクスチャ
	RenderTargetTexture* renderTargetTexture_;

	// ウインドウスプライト
	WindowSprite* windowSprite_;

	// ラジアルブラー実行するか
	bool runRadialBlur_;

	// 放射状ブラーの広がる強さ
	float radialBlurStrength_;

	// 放射状ブラーの広がる最大の強さ
	float radialBlurStrengthMax_;

	// 放射状ブラーがかかる速度
	float radialBlurSpeed_;

	// 放射状ブラーが最大でかかる速度
	float radialBlurSpeedMax_;

	// ドライブシステム
	DriveSystem* driveSystem_;

	// セットプレイヤー
	Player* player_;

};

