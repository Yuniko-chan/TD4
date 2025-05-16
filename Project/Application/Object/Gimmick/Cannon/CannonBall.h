#pragma once
#include "../../../../Engine/Object/MeshObject.h"
#include "cannonBallData.h"

/// <summary>
/// 大砲の弾
/// </summary>
class CannonBall :
    public MeshObject
{

private: // 定数

    // 生存時間
    static const float kLifeTime_;

    // 落下加速度
    static const float kFallingAcceleration_;

public: // 関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    CannonBall();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~CannonBall() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    /// <param name="cannonBallData">大砲の初期データ</param>
    void Initialize(LevelData::MeshData* data,
        const CannonBallData& cannonBallData);

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
    /// <param name="cannonBallData">大砲の弾</param>
    void Reset(const CannonBallData& cannonBallData);

    /// <summary>
    /// 動作しているか取得
    /// </summary>
    /// <returns></returns>
    bool GetIsWorking() { return isWorking_; }

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

    // 速度
    Vector3 velocity_;

    // 加速度
    Vector3 acceleration_;

    // 動作しているか
    bool isWorking_;

    // 生存時間経過
    float lifetimeElapsed_;

};
