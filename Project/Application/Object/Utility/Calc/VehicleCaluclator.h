#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"

class VehicleCaluclator
{
public:
	/// <summary>
	/// 加速度追加計算
	/// </summary>
	/// <returns></returns>
	//float AccelAddCalculator();

	Vector3 VelocityCalculator();
	/// <summary>
	/// 値を0に
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="epsilon"></param>
	/// <returns></returns>
	Vector3 SnapToZero(const Vector3& vector, const float epsilon);

	Vector3 RotateVector(const Vector3& velocity, const float euler);
	/// <summary>
	/// 指数関数補間
	/// </summary>
	/// <param name="current"></param>
	/// <param name="end"></param>
	/// <param name="decay"></param>
	/// <returns></returns>
	static Vector3 ExponentialInterpolate(const Vector3& current, const Vector3& end, float decay);
};
