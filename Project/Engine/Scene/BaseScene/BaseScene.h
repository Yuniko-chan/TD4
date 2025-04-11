#pragma once

// 基盤
#include "../../base/DxCommon/DirectXCommon.h" // DirectX関連
#include "../../Audio/Audio.h"// サウンド再生
#include "../../Input/Input.h"//入力デバイス
#include "../../base/Texture/TextureManager.h" // テクスチャマネージャー
#include "../../base/Debug/D3DResourceLeakChecker.h" // リソース解放確認
#include "../../2D/ImguiManager.h" // imGuiマネージャー
#include "../../GlobalVariables/GlobalVariables.h" // グローバル変数
#include "../../Level/LevelDataManager.h" // レベルデータ
#include "../../Object/BaseObjectManager.h" // オブジェクトマネージャー

// カメラ
#include "../../Camera/BaseCamera.h" // ベースカメラ
#include "../../Camera/DebugCamera.h"//デバッグカメラ

// 描画系
#include "../../2D/Sprite.h" // スプライト
#include "../../3D/Model/Model.h" // モデル
#include "../../3D/ManyObjects/LargeNumberOfObjects.h" // 大量のオブジェクト用モデル
#include "../../3D/Line/DrawLine.h" // 線描画
#include "../../3D/Material/Material.h"// マテリアル
#include "../../PostEffect/PostEffect.h" // ポストエフェクト
#include "../../../Engine/3D/Model/ModelDraw.h" // モデル描画
#include "../../../Engine/base/WindowSprite/WindowSprite.h" // ウインドウスプライト
#include "../../GPUParticle/GPUParticle.h" // GPUパーティクル
#include "../../3D/Model/ModelManager.h" // モデルマネージャー

// 数学系
#include "../../Math/DeltaTime.h" // デルタタイム
#include "../../Math/Ease.h" // 補間

// 衝突系
#include "../../Collision/BaseCollisionManager.h" // 3D衝突マネージャー
#include "../../Collider/ColliderDebugDraw/ColliderDebugDraw.h" // 3Dコライダーデバッグ描画
#include "../../../Engine/Collision2D/Collision2DManager.h" // 2D衝突マネージャー
#include "../../../Engine/Collision2D/Collision2DDebugDraw.h" // 2Dコライダーデバッグ描画

// 光源系
#include "../../../Engine/Light/DirectionalLight/DirectionalLight.h" // 平行光源
#include "../../../Engine/Light/PointLight/PointLightManager.h" // 点光源
#include "../../../Engine/Light/SpotLight/SpotLightManager.h" // スポット

// アプリケーション側
#include "../../../Application/Scene/SceneName.h" // シーンの名前

/// <summary>
/// シーンの元になるクラス
/// </summary>
class BaseScene
{

protected: // 静的メンバ変数

	// シーン番号
	static int sceneNo_;
	// リクエストシーン番号
	static int requestSceneNo_;

	// シーン番号確認
	static int sceneNoCheck_;
	// リクエストシーン番号確認
	static int requestSceneNoCheck_;

	// DirectXCommon
	static DirectXCommon* dxCommon_;
	// 入力
	static Input* input_;
	// オーディオ
	static Audio* audio_;
	// レンダーターゲットテクスチャ
	static RenderTargetTexture* renderTargetTexture_;

	// ビュープロジェクション
	static BaseCamera camera_;

	// デバッグカメラ
	static std::unique_ptr<DebugCamera> debugCamera_;
	// デバッグカメラを使用するか
	static bool isDebugCameraActive_;

	// 線描画
	static DrawLine* drawLine_;
	
	// レベルデータマネージャー
	static LevelDataManager* levelDataManager_;

	// モデルマネージャー
	static ModelManager* modelManager_;

public: // メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="levelDataManager">レベルデータマネージャー</param>
	static void StaticInitialize(LevelDataManager* levelDataManager);
	 
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseScene();

	/// <summary>
	/// シーン番号取得
	/// </summary>
	/// <returns></returns>
	int GetSceneNo();

	/// <summary>
	/// リクエストシーン番号取得
	/// </summary>
	/// <returns></returns>
	int GetRequestSceneNo();

	/// <summary>
	/// シーンをリセットするか取得
	/// </summary>
	/// <returns></returns>
	bool GetResetScene() { return resetScene_; }

	/// <summary>
	/// オーディオを止める
	/// </summary>
	/// <param name="stopAudio"></param>
	void SetStopAudio(bool stopAudio) { stopAudio_ = stopAudio; }

protected:  // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	virtual void ModelCreate();

	/// <summary>
	/// テクスチャロード
	/// </summary>
	virtual void TextureLoad();

	/// <summary>
	/// 初期化確認
	/// </summary>
	void InitilaizeCheck();

protected: // メンバ変数

	// シーンをリセット
	bool resetScene_ = false;

	// リセット中
	bool isBeingReset_ = false;

	// 削除された時オーディオ止める
	bool stopAudio_ = false;

	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_;
	// 平行光源データ
	DirectionalLightData directionalLightData_;

	// 点光源
	std::unique_ptr<PointLightManager> pointLightManager_;
	// 点光源データ
	std::array<PointLightData, PointLightManager::kNumInstanceMax_> pointLightDatas_;

	// スポットライト
	std::unique_ptr<SpotLightManager> spotLightManager_;
	// スポットライトデータ
	std::array<SpotLightData, SpotLightManager::kNumInstanceMax_> spotLightDatas_;

	// オブジェクトマネージャー
	std::unique_ptr<BaseObjectManager> objectManager_;

};

