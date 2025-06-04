#include "VehicleFactory.h"
#include "../CarLists.h"

Car::IParts* VehicleFactory::CreateParts(uint32_t number)
{
	Car::IParts* newParts = nullptr;
	switch (number)
	{
	case kEmpty:

		break;

	case kEngine:
		newParts = new EngineParts();
		break;

	case kFrame:
		newParts = new ArmorFrameParts();
		break;

	case kTire:
		newParts = new TireParts();
		break;

	default:
		break;
	}

    return newParts;
}
