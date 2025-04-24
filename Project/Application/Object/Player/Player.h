#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Animation/Animation.h"

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

    void SetPair(VehicleCore* pair) { pairCore_ = pair; }
    void SetParent();

private:
    /// <summary>
    /// コライダー更新
    /// </summary>
    void ColliderUpdate();

public: // ゲッター
    // コマンドの取得
    PlayerCommand* GetCommand() { return playerCommand_.get(); }
    // ステートマシンの取得
    PlayerStateMachine* GetStateMachine() { return stateMachine_.get(); }

    WorldTransform* GetCoreTransform();
private:
    // アニメーション
    std::unique_ptr<PlayerAnimation> playerAnimation_;
    std::unique_ptr<PlayerCommand> playerCommand_;
    std::unique_ptr<PlayerStateMachine> stateMachine_;

    VehicleCore* pairCore_ = nullptr;
};
