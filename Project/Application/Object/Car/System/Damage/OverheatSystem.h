#pragma once
#include "../VehicleSystemCommons.h"

class VehicleCore;

class OverheatSystem : public OwnerComponent<VehicleCore>
{
public:

	void Update();

	//void ImGuiDraw();


};