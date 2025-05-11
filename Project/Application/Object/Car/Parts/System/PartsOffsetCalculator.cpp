#include "PartsOffsetCalculator.h"

Vector3 PartsOffsetCalculator::GetOffset(VehicleConstructionSystem::Direction direct, int depth)
{
    const float kOffset = 2.0f;
	const float kOffsetValue = kOffset + (depth + 1);
	Vector3 result = {};
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
