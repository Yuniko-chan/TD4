#include "VehicleFactory.h"
#include "../CarLists.h"

Car::IParts* VehicleFactory::CreateParts(uint32_t number)
{
	Car::IParts* newParts = nullptr;
	switch (number)
	{
	case kEmpty:
		newParts = new EmptyParts();
		break;

	case kEngine:
		newParts = new EngineParts();
		break;

	case kFrame:

		break;

	case kTire:

		break;

	default:
		break;
	}

    return newParts;
}
