#pragma once
#include "../InterfacePickupPoint.h"

class TirePickupPoint : public IPickupPoint
{
public:
	void Initialize(LevelData::MeshData* data) override;

};
