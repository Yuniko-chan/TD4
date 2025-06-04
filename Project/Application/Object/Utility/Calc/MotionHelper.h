#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"

class MotionHelper
{
public:
	/// <summary>
	/// ジャンプアニメーション用カーブ処理
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="t"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	static Vector3 CurveBezier(const Vector3& start, const Vector3& end, float t, float height = 1.0f);

};
