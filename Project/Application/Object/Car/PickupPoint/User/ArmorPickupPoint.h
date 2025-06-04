#pragma once
#include "../InterfacePickupPoint.h"

class ArmorPickupPoint : public IPickupPoint
{
public:
	void Initialize(LevelData::MeshData* data) override;

};
