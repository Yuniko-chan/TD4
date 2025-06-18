#pragma once
#include "../../../../Engine/Object/MeshObject.h"
#include "MinigunData.h"
#include "MinigunBullet.h"
#include "MinigunAnimation.h"

/// <summary>
/// ミニガン
/// </summary>
class Minigun :
    public MeshObject
{

private: // 定数

    // 発射間隔
    static const float kFiringInterval_;

public: // 関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Minigun();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Minigun() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data,
        const MinigunData minigunData);

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
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

    /// <summary>
    /// コライダー登録
    /// </summary>
    /// <param name="collisionManager">衝突マネージャー</param>
    void CollisionListRegister(BaseCollisionManager* collisionManager);

    /// <summary>
    /// コライダー登録
    /// </summary>
    /// <param name="collisionManager">衝突マネージャー</param>
    /// <param name="colliderDebugDraw">衝突デバッグ描画</param>
    void CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw);

    /// <summary>
    /// 弾取得
    /// </summary>
    /// <returns></returns>
    std::list<MinigunBullet*>* GetBullets() { return &bullets_; }

private: // 関数

    /// <summary>
    /// コライダーの初期化
    /// </summary>
    /// <param name="collider">コライダー</param>
    void ColliderInitialize(ColliderShape collider) override;

    /// <summary>
    /// 撃つ
    /// </summary>
    void Fire();

private: // 変数

    // 弾（リスト）
    std::list<MinigunBullet*> bullets_;

    // 経過時間
    float elapsedTime_;

    // アニメーション
    std::unique_ptr<MinigunAnimation> animation_;

};

