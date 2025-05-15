#pragma once
#include "../../../Utility/Common/OwnerComponent.h"
#include "../../System/VehicleSystems.h"

class VehicleCore;

class DriveSystem : public OwnerComponent<VehicleCore>
{
public:
	//void Initialize();
	void Update();

};
