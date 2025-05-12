#pragma once
#include <cstdInt>

/// <summary>
/// 衝突マスク用の定数
/// </summary>

//プレイヤー
const uint32_t kCollisionAttributePlayer_ = 0b1;
//車両
const uint32_t kCollisionAttributeVehicleParts_ = 0b1 << 1;
//地形
const uint32_t kColisionAttributeTerrain = 0b1 << 2;
//ギミック
const uint32_t kColisionAttributeGimmick = 0b1 << 3;