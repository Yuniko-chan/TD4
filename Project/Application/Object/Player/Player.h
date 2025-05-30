#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Animation/Animation.h"
#include "../../Camera/FollowCamera.h"
#include "../../Camera/GameCameraManager.h"

#include "PlayerSystemLists.h"

class VehicleCore;

class Player : public MeshObject
{
public: // メンバ関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Player();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Player() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    void Draw(BaseCamera& camera) override;

    /// <summary>
    /// ImGui描画
    /// </summary>
    void ImGuiDraw() override;

    /// <summary>
    /// パーティクル描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    void ParticleDraw(BaseCamera& camera) override;

    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

    void SettingParent();
    // 親の確認
    bool IsParent() { return worldTransform_.parent_ ? true : false; }

private:
    /// <summary>
    /// コライダー更新
    /// </summary>
    void ColliderUpdate();
    /// <summary>
    /// システムクラス関係の初期化
    /// </summary>
    void SystemInitialize();
    /// <summary>
    /// システムクラスの更新
    /// </summary>
    void SystemUpdate();
public: // アクセッサ（プレイヤーシステム）
    // コマンドの取得
    PlayerCommand* GetCommand() { return playerCommand_.get(); }
    // ステートマシンの取得
    PlayerStateMachine* GetStateMachine() { return stateMachine_.get(); }
    // パーツの管理クラス取得
    PlayerPickupManager* GetPickUpManager() { return pickUpManager_.get(); }
    // フロントチェックの取得
    PlayerFrontChecker* GetFrontChecker() { return &frontChecker_; }

public: // アクセッサ（プレイヤーシステム以外）
    //---ゲッター---//
    // コアのワールドトランスフォーム
    WorldTransform* GetCoreTransform();
    // コア
    VehicleCore* GetCore() { return pairCore_; }
    // カメラ
    FollowCamera* GetCamera() { return followCamera_; }
    GameCameraManager* GetCameraManager() { return cameraManager_; }
    //---セッター---//
    void SetPair(VehicleCore* pair) { pairCore_ = pair; }
    void SetCamera(FollowCamera* camera) { followCamera_ = camera; }
    void SetCameraManager(GameCameraManager* cameraManager) { cameraManager_ = cameraManager; }
private:
    // システムクラス
    std::unique_ptr<PlayerAnimation> playerAnimation_;
    std::unique_ptr<PlayerCommand> playerCommand_;
    std::unique_ptr<PlayerStateMachine> stateMachine_;
    std::unique_ptr<PlayerPickupManager> pickUpManager_;
    // デバッグ用のデータ
    PlayerDebugData debugData_;
    // 前方確認
    PlayerFrontChecker frontChecker_;
    // ペア
    VehicleCore* pairCore_ = nullptr;
    // カメラ
    FollowCamera* followCamera_ = nullptr;
    // カメラ管理クラス
    GameCameraManager* cameraManager_ = nullptr;
};
