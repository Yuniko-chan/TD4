#pragma once
#include <array>

#include "../../../Engine/Scene/BaseScene/BaseScene.h"

#include "../../../Engine/Light/DirectionalLight/DirectionalLight.h" // 平行光源
#include "../../../Engine/Light/PointLight/PointLightManager.h" // 点光源
#include "../../../Engine/Light/SpotLight/SpotLightManager.h" // スポット

#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../Engine/Collision2D/Collision2DDebugDraw.h"

#include "../../Camera/FollowCamera.h"
#include "../../Camera/GameCameraManager.h"
#include "../../Effect/EffectManager.h"
#include "../../Collision/GameSceneCollisionManager.h"
#include "../../UI/UIManager.h"
#include "../../System/PostEffectSystem/PostEffectSystem.h"
#include "../../Object/KeyConfig/GameKeyconfig.h"
#include "../../Object/GlobalParameterManager.h"
#include "../../Object/GameTimer/GameTimeSystem.h"
#include "../../Course/CourseCollisionSystem.h"
#include "../../Course/Debug/CourseDebugDraw.h"
#include "../../Course/CourseManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : 
	public BaseScene
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

public: //メンバ関数

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
	void DebugCameraUpdate();

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

	/// <summary>
	/// コース初期化
	/// </summary>
	void CourseInitialize();

	//コース追加(デバッグ用)
	void AddCourse();

private:

	// パーティクルモデル
	std::unique_ptr<Model> particleCircleModel_ = nullptr;

	// 衝突マネージャー
	std::unique_ptr<GameSceneCollisionManager> collisionManager_;

	// 追加カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// カメラマネージャ
	std::unique_ptr<GameCameraManager> cameraManager_;

	// エフェクトマネージャー
	EffectManager* effectManager_ = nullptr;

	// UIマネージャー
	std::unique_ptr<UIManager> uiManager_ = nullptr;

	// ポストエフェクトシステム
	std::unique_ptr<PostEffectSystem> postEffectSystem_ = nullptr;

	// キー入力管理
	GameKeyconfig* keyConfig_ = nullptr;
	// 外部パラメータ変数をまとめておくクラス
	GlobalParameterManager* parameterManager_ = nullptr;
	// ゲーム内時間管理
	std::unique_ptr<GameTimeSystem> gameTimeSystem_;

	// コース衝突システム
	std::unique_ptr<CourseCollisionSystem> courseCollisionSystem_;
	// コースデバッグ描画
	std::unique_ptr<CourseDebugDraw> courseDebugDraw_;

	//コースの生成などを行うクラス
	std::unique_ptr<CourseManager> courseManager_;

};
