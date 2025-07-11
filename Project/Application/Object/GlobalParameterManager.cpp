#include "GlobalParameterManager.h"

void GlobalParameterManager::Initialize()
{
	globalVariables_ = GlobalVariables::GetInstance();

	// アイテムの初期化
	AddItems();
	// パラメータ
	ApplyGlobalVariables();
}

void GlobalParameterManager::Update()
{
	// 更新
	ApplyGlobalVariables();
}

void GlobalParameterManager::ApplyGlobalVariables()
{

	const char* groupName = "Player";
	player_.rideSpeedFactor = globalVariables_->GetFloatValue(groupName, "RideSpeed");
	player_.walkSpeedFactor = globalVariables_->GetFloatValue(groupName, "WalkSpeed");

	player_.ride.jumpHeight = globalVariables_->GetFloatValue(groupName, "RideHeight");
	player_.ride.actionFrame = globalVariables_->GetFloatValue(groupName, "RideActionFrame");
	player_.ride.offset = globalVariables_->GetVector3Value(groupName, "RideOffset");

	player_.dropOff.jumpHeight = globalVariables_->GetFloatValue(groupName, "DropOffHeight");
	player_.dropOff.actionFrame = globalVariables_->GetFloatValue(groupName, "DropOffActionFrame");
	player_.dropOff.offset = globalVariables_->GetVector3Value(groupName, "DropOffOffset");

	player_.frontCheck.threshold = globalVariables_->GetFloatValue(groupName, "FrontThreshold");
	player_.frontCheck.catchRange = globalVariables_->GetFloatValue(groupName, "FrontCatchRange");

	player_.cameraRotateSpeed = globalVariables_->GetFloatValue(groupName, "CameraRotateSpeed");

	player_.holdOffset = globalVariables_->GetVector3Value(groupName, "HoldOffset");

	//groupName = "CameraCommon";
	//cameraCommon_.inVehicle.first = globalVariables_->GetVector3Value(groupName, "InVehicleOffset");
	//cameraCommon_.inVehicle.second = globalVariables_->GetVector3Value(groupName, "InVehicleRotation");

	//cameraCommon_.onFoot.first = globalVariables_->GetVector3Value(groupName, "OnFootOffset");
	//cameraCommon_.onFoot.second = globalVariables_->GetVector3Value(groupName, "OnFootRotation");

	groupName = "OverheadCamera";
	overheadCamera_.defaultData.ApplyGlobalVariables(groupName, globalVariables_);

	groupName = "DriveCamera";
	driveCamera_.defaultData.ApplyGlobalVariables(groupName, globalVariables_);
	driveCamera_.backTrackingDelay = globalVariables_->GetFloatValue(groupName, "BackTrackingDelay");

}

void GlobalParameterManager::AddItems()
{
	const char* groupName = "Player";

	// 乗る
	globalVariables_->AddItem(groupName, "RideHeight", float(player_.ride.jumpHeight));
	globalVariables_->AddItem(groupName, "RideActionFrame", float(player_.ride.actionFrame));
	globalVariables_->AddItem(groupName, "RideOffset", Vector3(player_.ride.offset));
	// 降りる
	globalVariables_->AddItem(groupName, "DropOffHeight", float(player_.dropOff.jumpHeight));
	globalVariables_->AddItem(groupName, "DropOffActionFrame", float(player_.dropOff.actionFrame));
	globalVariables_->AddItem(groupName, "DropOffOffset", Vector3(player_.dropOff.offset));
	// 拾う
	globalVariables_->AddItem(groupName, "FrontThreshold", float(player_.frontCheck.threshold));
	globalVariables_->AddItem(groupName, "FrontCatchRange", float(player_.frontCheck.catchRange));

	globalVariables_->AddItem(groupName, "HoldOffset", Vector3(player_.holdOffset));

	// 移動量
	globalVariables_->AddItem(groupName, "RideSpeed", float(player_.rideSpeedFactor));
	globalVariables_->AddItem(groupName, "WalkSpeed", float(player_.walkSpeedFactor));

	//---カメラ---//
	globalVariables_->AddItem(groupName, "CameraRotateSpeed", float(player_.cameraRotateSpeed));
	// 共通
	//groupName = "CameraCommon";
	//globalVariables_->AddItem(groupName, "InVehicleOffset", Vector3(cameraCommon_.inVehicle.first));
	//globalVariables_->AddItem(groupName, "InVehicleRotation", Vector3(cameraCommon_.inVehicle.second));
	//globalVariables_->AddItem(groupName, "OnFootOffset", Vector3(cameraCommon_.onFoot.first));
	//globalVariables_->AddItem(groupName, "OnFootRotation", Vector3(cameraCommon_.onFoot.second));
	// 俯瞰
	groupName = "OverheadCamera";
	overheadCamera_.defaultData.Initialize(groupName, globalVariables_);
	// 運転
	groupName = "DriveCamera";
	driveCamera_.defaultData.Initialize(groupName, globalVariables_);
	globalVariables_->AddItem(groupName, "BackTrackingDelay", float(driveCamera_.backTrackingDelay));
}
