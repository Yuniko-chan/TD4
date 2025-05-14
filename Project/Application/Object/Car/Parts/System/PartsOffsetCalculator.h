#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Utility/Math/Vector2Int.h"
#include "../../Construction/VehicleConstructionSystem.h"

class PartsOffsetCalculator
{
public:
	// オフセット計算
	Vector3 GetOffset(VehicleConstructionSystem::Direction direct, int depth);
	Vector3 GetOffset(const Vector2Int& id);
};
