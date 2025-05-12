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

};

