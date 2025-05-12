#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../Construction/VehicleConstructionSystem.h"

class PartsOffsetCalculator
{
public:
	// オフセット計算
	Vector3 GetOffset(VehicleConstructionSystem::Direction direct, int depth);

};
