#pragma once
#include "../../../../Engine/Object/MeshObject.h"
#include "PendulumIronBallData.h"

/// <summary>
/// 振り子鉄球
/// </summary>
class PendulumIronBall :
    public MeshObject
{

public: // 関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    PendulumIronBall();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~PendulumIronBall() override;

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
        const PendulumIronBallData& conicalPendulumIronBallData);

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
    /// 紐のワールドトランスフォーム取得
    /// </summary>
    /// <returns></returns>
    WorldTransform* GetStringWorldTransformAddress() { return &stringWorldTransform_; }

    /// <summary>
    /// 紐のマテリアル取得
    /// </summary>
    /// <returns></returns>
    Material* GetStringMaterial() { return stringMaterial_.get(); }

private: // 関数

    /// <summary>
    /// コライダーの初期化
    /// </summary>
    /// <param name="collider">コライダー</param>
    void ColliderInitialize(ColliderShape collider) override;

    /// <summary>
    /// コライダー更新
    /// </summary>
    void ColliderUpdate();

    /// <summary>
    /// 振り子初期化
    /// </summary>
    /// <param name="length">振り子の初期データ</param>
    void PendulumInitialize(const PendulumIronBallData& conicalPendulumIronBallData);

    /// <summary>
    /// 振り子更新
    /// </summary>
    void PendulumUpdate();

    /// <summary>
    /// 紐初期化
    /// </summary>
    void StringInitialize();

    /// <summary>
    /// 紐更新
    /// </summary>
    void StringUpdate();

private: // 変数

    // アンカーポイント
    Vector3 anchor_;
    // 紐の長さ
    float length_;
    //　現在の角度
    float angle_;
    //角速度
    float angularVelocity_;
    // 角加速度
    float angularAcceleration_;

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

