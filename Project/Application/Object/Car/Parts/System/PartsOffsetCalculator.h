#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Utility/Math/Vector2Int.h"
#include "../../System/Construction/VehicleConstructionSystem.h"

class PartsOffsetCalculator
{
public:
	// オフセット計算
	Vector3 GetOffset(const Vector2Int& id);
};
