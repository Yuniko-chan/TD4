#include "GlobalParameterStructs.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void ParameterStructs::Datas::CameraDefault::Initialize(const char* groupName, GlobalVariables* globalVariables)
{
	globalVariables->AddItem(groupName, "Position", Vector3(this->position));
	globalVariables->AddItem(groupName, "RotateVector", Vector3(this->rotateVector));
	globalVariables->AddItem(groupName, "TrackingDelay", float(this->trackingDelay));
}

void ParameterStructs::Datas::CameraDefault::ApplyGlobalVariables(const char* groupName, GlobalVariables* globalVariables)
{
	position = globalVariables->GetVector3Value(groupName, "Position");
	rotateVector = globalVariables->GetVector3Value(groupName, "RotateVector");
	trackingDelay = globalVariables->GetFloatValue(groupName, "TrackingDelay");
}
