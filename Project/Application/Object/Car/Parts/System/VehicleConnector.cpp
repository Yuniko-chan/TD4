#include "VehicleConnector.h"
#include "../PartsInterface.h"

void VehicleConnector::Update()
{
	// 親がなければ
	if (parents_.empty()) {
		owner_->SetIsDelete(true);
	}

}
