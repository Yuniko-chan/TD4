#pragma once
#include "../../../../Engine/Object/MeshObject.h"
#include "ConicalPendulumIronBallData.h"

/// <summary>
/// 円錐振り子鉄球
/// </summary>
class ConicalPendulumIronBall :
    public MeshObject
{

public: // 関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    ConicalPendulumIronBall();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~ConicalPendulumIronBall() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    /// <param name="length">円錐振り子の初期データ</param>
    void Initialize(LevelData::MeshData* data, 
        const ConicalPendulumIronBallData& conicalPendulumIronBallData);

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

private: // 関数

    /// <summary>
    /// コライダーの初期化
    /// </summary>
    /// <param name="collider">コライダー</param>
    void ColliderInitialize(ColliderShape collider) override;

    /// <summary>
    /// 円錐振り子初期化
    /// </summary>
    /// <param name="length">円錐振り子の初期データ</param>
    void ConicalPendulumInitialize(const ConicalPendulumIronBallData& conicalPendulumIronBallData);

    /// <summary>
    /// 円錐振り子更新
    /// </summary>
    void ConicalPendulumUpdate();

    /// <summary>
    /// 紐初期化
    /// </summary>
    void StringInitialize();

    /// <summary>
    /// 紐更新
    /// </summary>
    void StringUpdate();

private: // 変数

    // アンカー
    Vector3 anchor_;
    // 紐の長さ
    float length_;
    // 円推の頂角の半分
    float halfApexAngle_;
    // 現在の角度
    float angle_;
    // 各速度
    float angularVelocity_;

    // 紐描画
    // ファイル名前
    std::string stringFileName_;
    // モデル
    Model* stringModel_ = nullptr;
    // マテリアル
    std::unique_ptr<Material> stringMaterial_;
    // トランスフォーム
    WorldTransform stringWorldTransform_;

};

