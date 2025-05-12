#pragma once
#include "../../../../Engine/Math/Vector/Vector3.h"

/// <summary>
/// 円錐振り子鉄球データ
/// </summary>
struct ConicalPendulumIronBallData
{

    // アンカー
    Vector3 anchor;
    // 紐の長さ
    float length;
    // 円推の頂角の半分
    float halfApexAngle;

};
