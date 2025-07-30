#pragma once
#include "../../../Engine/Scene/BaseScene/BaseScene.h"
#include "../../../Engine/PostEffect/HSV/HSVFilter.h"
#include "../../TitleSceneObject/TitleSpriteObjects.h"
#include "../../AudioManager/TitleAudioManager.h"
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : 
	public BaseScene
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

private: // メンバ関数

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
 	void DebugCameraUpdate();

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private: // メンバ変数

	// タイトルスプライトオブジェクト
	std::unique_ptr<TitleSpriteObjects> titleSpriteObjects_;

	// オーディオマネージャー
	std::unique_ptr<TitleAudioManager> audioManager_;

};
