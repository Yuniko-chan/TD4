#pragma once
#include "../PartsInterface.h"

class EmptyParts : public Car::IParts
{
public:
	void ImGuiDrawParts() override {};
};
