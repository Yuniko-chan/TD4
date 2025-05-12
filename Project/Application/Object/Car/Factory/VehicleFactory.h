#pragma once
#include <memory>
#include <cstdint>

namespace Car {
	class IParts;
}

class VehicleFactory
{
protected:
	enum PartsNumber
	{
		kEmpty,
		kEngine,
		kFrame,
		kTire,
		kCountOf,
	};

public:
	virtual ~VehicleFactory() = default;
	virtual Car::IParts* CreateParts(uint32_t number);

};
