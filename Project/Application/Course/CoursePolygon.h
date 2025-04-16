#pragma once
#include "../../Engine/Math/Vector/Vector3.h"

/// <summary>
/// コースポリゴン
/// </summary>
struct CoursePolygon
{

	// 位置3つ
	Vector3 positions[3];

	// 法線
	Vector3 normal;

	// コースポリゴン種類
	uint32_t coursePolygonType;

};