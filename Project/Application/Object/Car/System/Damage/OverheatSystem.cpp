#include "OverheatSystem.h"
#include "../../VehicleCore.h"

void OverheatSystem::Update()
{
	// オーバーヒートしてなければ早期
	if (!owner_->GetStatus()->GetIsOverheat()) {
		return;
	}


}
