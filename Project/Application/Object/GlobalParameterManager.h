#pragma once
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "Utility/Common/SingletonModule.h"

class GlobalParameterManager : public SingletonModule<GlobalParameterManager>
{
public:
	void Initialize();
	void Update();
	void ApplyGlobalVariables();

private:
	GlobalVariables* globalVariables_ = nullptr;
};
