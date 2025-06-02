#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/Math/Vector/Vector2.h"

/// <summary>
/// トランスフォーム関係の計算関数クラス
/// </summary>
class TransformHelper
{
public:
	/// <summary>
	/// ワールドから分離する処理（親子）
	/// </summary>
	/// <param name="parentRotate"></param>
	/// <param name="child"></param>
	/// <returns></returns>
	static EulerTransform DetachWithWorldTransform(const Vector3& parentRotate, WorldTransform* child);
	static EulerTransform DetachWithWorldTransform(WorldTransform* child);

	/// <summary>
	/// 3次元の距離
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static float Vector3Distance(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// XZ平面上で回転
	/// </summary>
	/// <param name="direct"></param>
	/// <param name="theta"></param>
	/// <returns></returns>
	static Vector3 XZRotateDirection(const Vector3& direct, float& theta);

	/// <summary>
	/// XZベクトルの回転角を求める関数
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns></returns>
	static float CalculateXZVectorToRotateRadian(const Vector3& from, const Vector3& to);

	static Vector3 XZRotation(const Vector3& direct, float& theta);
};