#pragma once
#include "../InterfacePickupPoint.h"

class EnginePickupPoint : public IPickupPoint
{
public:
	void Initialize(LevelData::MeshData* data) override;

};
