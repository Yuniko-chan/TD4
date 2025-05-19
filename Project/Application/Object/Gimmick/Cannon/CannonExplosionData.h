#pragma once

/// <summary>
/// 大砲爆発データ
/// </summary>
struct CannonExplosionData
{

    // 爆発時間
    float explosionTime_;

    // 爆発最大半径
    float explosionRadiusMax;

    // 半径が最大になるまでの時間
    float timeToReachRadiusMax;

    // 位置
    Vector3 position;

};