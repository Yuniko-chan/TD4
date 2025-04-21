#include "VehicleFactory.h"
#include "../CarLists.h"

std::unique_ptr<Car::IParts> VehicleFactory::CreateParts(uint32_t number)
{
    std::unique_ptr<Car::IParts> newParts;
	switch (number)
	{
	case kEmpty:
		newParts = std::make_unique<EmptyParts>();
		break;

	case kEngine:
		newParts = std::make_unique<EngineParts>();
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
