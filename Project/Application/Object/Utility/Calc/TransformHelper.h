#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"

class TransformHelper
{
public:
	/// <summary>
	/// 
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
};