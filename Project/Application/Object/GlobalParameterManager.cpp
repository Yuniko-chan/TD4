#include "GlobalParameterManager.h"

void GlobalParameterManager::Initialize()
{
	globalVariables_ = GlobalVariables::GetInstance();

	//const char* groupName = "";

	ApplyGlobalVariables();
}

void GlobalParameterManager::Update()
{
	ApplyGlobalVariables();
}

void GlobalParameterManager::ApplyGlobalVariables()
{

}
