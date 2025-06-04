#pragma once
#include "../../../../Engine/Object/MeshObject.h"
#include "CannonExplosionData.h"

/// <summary>
/// 大砲爆発
/// </summary>
class CannonExplosion :
    public MeshObject
{

public: // 関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    CannonExplosion();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~CannonExplosion() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    /// <param name="cannonData">大砲の初期データ</param>
    void Initialize(LevelData::MeshData* data,
        const CannonExplosionData& cannonData);

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
    /// リセット
    /// </summary>
    /// <param name="cannonExplosionData">大砲爆発データ</param>
    void Reset(const CannonExplosionData& cannonExplosionData);

private: // 関数

    /// <summary>
    /// コライダーの初期化
    /// </summary>
    /// <param name="collider">コライダー</param>
    void ColliderInitialize(ColliderShape collider) override;

    /// <summary>
    /// コライダーの更新
    /// </summary>
    void ColliderUpdate();

private: // 変数

    // 爆発してるか
    bool isExploding_;

    // 爆発時間
    float explosionTime_;

    // 経過時間
    float elapsedTime_;

    // 爆発半径
    float explosionRadius_;

    // 爆発最大半径
    float explosionRadiusMax_;

    // 半径が最大になるまでの時間
    float timeToReachRadiusMax_;

};

