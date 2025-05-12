#include "PartsOffsetCalculator.h"

Vector3 PartsOffsetCalculator::GetOffset(VehicleConstructionSystem::Direction direct, int depth)
{
	// オフセット用の値
    const float kOffset = 2.0f;
	const float kOffsetValue = kOffset * (depth);
	Vector3 result = {};
	// 方向ごと
	switch (direct)
	{
	case VehicleConstructionSystem::kLeft:
		result.x = -(kOffsetValue);
		break;
	case VehicleConstructionSystem::kRight:
		result.x = (kOffsetValue);
		break;
	case VehicleConstructionSystem::kForward:
		result.z = (kOffsetValue);
		break;
	case VehicleConstructionSystem::kBackForward:
		result.z = -(kOffsetValue);
		break;
	default:
		break;
	}

	// 
    return Vector3(result);
}
