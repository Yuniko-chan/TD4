#pragma once
#include "../../../../Engine/Object/MeshObject.h"
#include "MinigunBulletData.h"

/// <summary>
/// ミニガンの弾
/// </summary>
class MinigunBullet :
    public MeshObject
{

private: // 定数

    // 生存時間
    static const float kLifeTime_;
    // 速度
    static const float kSpeed_;

public: // 関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    MinigunBullet();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~MinigunBullet() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    /// <param name="minigunBulletData">データ</param>
    void Initialize(LevelData::MeshData* data, const MinigunBulletData minigunBulletData);

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

    /// <summary>
    /// 死んだか
    /// </summary>
    /// <returns></returns>
    bool IsDead() const { return isDead_; };

private: // 関数

    /// <summary>
    /// コライダーの初期化
    /// </summary>
    /// <param name="collider">コライダー</param>
    void ColliderInitialize(ColliderShape collider) override;

private: // 変数

    // 経過時間
    float elapsedTime_;

    // 死亡しているか
    bool isDead_;

    // 速度
    Vector3 velocity_;

};

