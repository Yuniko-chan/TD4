#pragma once
#include <string>
#include <array>
#include <algorithm>

namespace VehicleDatas
{
	enum PartIndex {
		kCore,
		kEngine,
		kTire,
		kArmor,
		kSize,
	};


}

using PathPair = std::pair<std::string, std::string>;
const static std::array<PathPair, VehicleDatas::PartIndex::kSize> sVehiclePaths = {
	PathPair{"Resources/Model/Frame","Frame.obj"},
	PathPair{"Resources/Model/Engine","Engine.obj"},
	PathPair{"Resources/Model/Tire","Tire.obj"},
	PathPair{"Resources/Model/Frame","Frame.obj"},
};
