#pragma once
#include "../../Engine/Math/Vector/Vector3.h"
#include "../../Engine/Math/Vector/Vector2.h"

/// <summary>
/// コースポリゴン
/// </summary>
struct CoursePolygon
{

	// 位置3つ
	Vector3 position0;
	Vector3 position1;
	Vector3 position2;
	
	// 法線
	Vector3 normal;

	// テクスチャ座標
	Vector2 texcoord;

};