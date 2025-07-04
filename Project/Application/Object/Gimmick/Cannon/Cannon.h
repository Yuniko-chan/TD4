#pragma once
#include "../../../../Engine/Object/MeshObject.h"
#include "CannonData.h"
#include "CannonBall.h"

/// <summary>
/// 大砲
/// </summary>
class Cannon :
    public MeshObject
{

public: // 関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Cannon();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Cannon() override;

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
        const CannonData& cannonData);

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

    /// <summary>
    /// 大砲の初期化
    /// </summary>
    /// <param name="cannonData"></param>
    void CannonInitialize(const CannonData& cannonData);

    /// <summary>
    /// 大砲の更新
    /// </summary>
    void CannonUpdate();

    /// <summary>
    /// クールタイム更新
    /// </summary>
    void CooltimeUpdate();

    /// <summary>
    /// 弾発射
    /// </summary>
    void BulletFiring();

    /// <summary>
    /// 大砲の弾初期化
    /// </summary>
    void CannonBallInitialize();

public: // 関数

    /// <summary>
    /// 大砲の弾取得
    /// </summary>
    /// <returns></returns>
    CannonBall* GetCannonBall() { return cannonBall_.get(); }

private: // 変数

    // 撃つ状態か
    bool readyToFire_;

    // クールタイム経過
    float cooltimeElapsed_;

    // クールタイム値
    float cooltimeMax_;

    // 撃つ
    bool isFiring_;

    // 大砲の弾
    std::unique_ptr<CannonBall> cannonBall_;

    // 発射向き
    Vector3 firingDirection_;

    // 発射速度
    float firingSpeed_;

};

