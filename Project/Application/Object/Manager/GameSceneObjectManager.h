#pragma once
#include "../../../Engine/Object/BaseObjectManager.h"
#include "../../System/Shadow/ShadowManager.h"
#include "../Car/Manager/VehiclePartsManager.h"
#include "../Car/Manager/PickupPointManager.h"
#include "../../System/Draw/GameSceneInstancingDrawing.h"

// 前方宣言
class Player;

/// <summary>
/// ゲームシーン用のオブジェクトマネージャー
/// </summary>
class GameSceneObjectManager :
    public BaseObjectManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="levelIndex">レベル番号</param>
	/// <param name="levelDataManager">レベルデータマネージャー</param>
	void Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(BaseCamera& camera) override;

	/// <summary>
	/// 描画(デバッグ)
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="drawLine">線描画クラス</param>
	void Draw(BaseCamera& camera, DrawLine* drawLine) override;

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;
private: // メンバ関数

	/// <summary>
	/// 影更新
	/// </summary>
	void ShadowUpdate();


	// プレイヤー専用の初期化処理
	void PlayerInitialize();
	void OptionProcess();


	void VehiclePreset(const std::string& presetName);
public:

	//オブジェクトデータからの取得ポイントの登録
	void RegisterPickupPoint(IObject* object, const std::string& className, const std::string& objectName);

	/// <summary>
	/// オブジェクト追加（ハードコーディング用）
	/// </summary>
	/// <param name="className"></param>
	/// <param name="directory"></param>
	/// <param name="modelName"></param>
	void AddObject(const std::string& className, const std::string& directory, const std::string& modelName);
	void AddObject(const std::string& className, const std::string& name, const std::string& directory, const std::string& modelName);
	void AddObject(const std::string& className, const std::string& name, const std::string& directory, const std::string& modelName, const Vector3& position);

	void AddObject(IObject* object);
public:
	// パーツ用のマネージャークラス取得
	VehiclePartsManager* GetPartsManager() { return partsManager_.get(); }

private: // メンバ変数

	// 影マネージャー
	std::unique_ptr<ShadowManager> shadowManager_;
	// 影モデル
	std::unique_ptr<Model> shadowModel_;

	// レベルデータマネージャー
	LevelDataManager* levelDataManager_;

	// インスタンシング描画
	std::unique_ptr<GameSceneInstancingDrawing> instancingDrawing_;

private: // USER
	// パーツマネージャー
	std::unique_ptr<VehiclePartsManager> partsManager_;
	// 拾う場所のマネージャー
	std::unique_ptr<PickupPointManager> pickupPointManager_;
};

