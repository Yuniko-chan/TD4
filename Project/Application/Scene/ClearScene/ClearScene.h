#pragma once
#include "../../../Engine/Scene/BaseScene/BaseScene.h"
#include "../../ClearSceneObject/CourseTraversalSystem.h"

/// <summary>
/// クリアシーン
/// </summary>
class ClearScene : 
	public BaseScene
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ClearScene();

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
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private: // メンバ変数

	// コース踏破システム
	std::unique_ptr<CourseTraversalSystem> courseTraversalSystem_;

};

