#include "VehicleConnector.h"
#include "../PartsInterface.h"

void VehicleConnector::Update()
{
	for (std::list<Car::IParts*>::iterator it = parents_.begin(); it != parents_.end(); ++it) {
		//(*it)->get
	}

}
