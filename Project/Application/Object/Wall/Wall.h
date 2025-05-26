#pragma once
#include "../../../Engine/Object/MeshObject.h"

/// <summary>
/// 壁
/// </summary>
class Wall :
    public MeshObject
{

public: // 関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Wall();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Wall() override;

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

private: // 関数

    /// <summary>
    /// コライダーの初期化
    /// </summary>
    /// <param name="collider">コライダー</param>
    void ColliderInitialize(ColliderShape collider) override;

private: // 変数

    bool isVisible_ = false;

};

