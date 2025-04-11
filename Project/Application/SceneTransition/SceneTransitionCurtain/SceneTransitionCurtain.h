#pragma once
#include "../../../Engine/SceneTransition/BaseSceneTransition/BaseSceneTransition.h"
#include <memory>
#include "../../../Engine/2D/Sprite.h"
#include "../../../Engine/Camera/BaseCamera.h"
#include "../../OutGameObject/Curtain/Curtain.h"

/// <summary>
/// シーン遷移カーテンver
/// </summary>
class SceneTransitionCurtain :
    public BaseSceneTransition
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneTransitionCurtain() override;

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

private: 

	// カメラ
	BaseCamera camera_;

	// カーテン
	std::unique_ptr<Curtain> curtain_;

	// 黒スプライト
	std::unique_ptr<Sprite> blockSprite_;
	// 黒テクスチャハンドル
	uint32_t blockTextureHandle_;
	// 黒色（透明度も含める）
	Vector4 blockColor_;

};

