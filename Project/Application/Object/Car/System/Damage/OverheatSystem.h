#pragma once
#include "../VehicleSystemCommons.h"

class VehicleCore;

class OverheatSystem : public OwnerComponent<VehicleCore>
{
public:

	void Update();

	void SetMappingData(std::map<Vector2Int, Car::IParts*>* data) { mappingData_ = data; }

private:
	// パーツのマッピングデータ
	std::map<Vector2Int, Car::IParts*>* mappingData_ = nullptr;

};