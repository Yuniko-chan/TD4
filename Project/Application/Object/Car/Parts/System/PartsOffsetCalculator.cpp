#include "PartsOffsetCalculator.h"

Vector3 PartsOffsetCalculator::GetOffset(const Vector2Int& id)
{
	const float kOffset = 2.0f;
	Vector3 result = {};
	result.x = float(id.x) * kOffset;
	result.z = float(id.y) * kOffset;
	return Vector3(result);
}
