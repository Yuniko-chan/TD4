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
};