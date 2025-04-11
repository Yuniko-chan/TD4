#pragma once
#include "../../Math/Vector/Vector2.h"

/// <summary>
/// 衝撃波データ
/// </summary>
struct ShockWaveData
{

	Vector2 center; // 中心
	float distortion; // 歪み
	float radius; // 半径
	float thickness; // 厚み

};
