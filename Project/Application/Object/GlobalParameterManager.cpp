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
	
	// 車両
	groupName = "Vehicle";
	vehicle_.core.initPosition = globalVariables_->GetVector3Value(groupName, "Core_InitPosition");
	vehicle_.core.inVehicleLocal = globalVariables_->GetVector3Value(groupName, "Core_LocalPosition");

	vehicle_.overheat.maxDPS = globalVariables_->GetFloatValue(groupName, "OverheatMaxDamage");
	vehicle_.overheat.minDPS = globalVariables_->GetFloatValue(groupName, "OverheatMinDamage");
	vehicle_.overheat.pushAngleThreshold = globalVariables_->GetFloatValue(groupName, "PushAngleThreshold");
	vehicle_.overheat.pushForcePerEngine = globalVariables_->GetFloatValue(groupName, "PushForcePerEngine");
	vehicle_.overheat.knockbackDecayFactor = globalVariables_->GetFloatValue(groupName, "KnockbackDecayFactor");


	//---エンジン関係---//
	groupName = "VehicleEngine";
	vehicle_.engine.inputInterval = globalVariables_->GetFloatValue(groupName, "InputInterval");
	vehicle_.engine.inputDecrementInterval = globalVariables_->GetFloatValue(groupName, "InputDecrementInterval");
	vehicle_.engine.maxInputCount = globalVariables_->GetIntValue(groupName, "InputMaxCount");
	// 摩擦
	vehicle_.engine.roadFriction = globalVariables_->GetFloatValue(groupName, "RoadFriction");
	vehicle_.engine.dirtFriction = globalVariables_->GetFloatValue(groupName, "DirtFriction");
	// 加速
	vehicle_.engine.maxEngineCountAccelFactor = globalVariables_->GetFloatValue(groupName, "MaxEngineCountAccelFactor");
	vehicle_.engine.minEngineCountAccelFactor = globalVariables_->GetFloatValue(groupName, "MinEngineCountAccelFactor");
	vehicle_.engine.accelerationMultiplier = globalVariables_->GetFloatValue(groupName, "AccelerationMultiplier");
	
	// 減速
	vehicle_.engine.idleDecelerationFactor = globalVariables_->GetFloatValue(groupName, "IdleDecelerationFactor");
	vehicle_.engine.stopDecelerationRate = globalVariables_->GetFloatValue(groupName, "StopDecelerationFactor");
	// 最大
	vehicle_.engine.maxEffectiveCount = (int)globalVariables_->GetIntValue(groupName, "MaxEffectiveCount");
	vehicle_.engine.overheatEngineThreshold = (int)globalVariables_->GetIntValue(groupName, "OverheatEngineCountThreshold");

	groupName = "VehicleHandling";
	vehicle_.handling.steerMaxAngle = globalVariables_->GetFloatValue(groupName, "SteerMaxAngle");
	vehicle_.handling.steerMinAngle = globalVariables_->GetFloatValue(groupName, "SteerMinAngle");

	vehicle_.handling.inputInterval = globalVariables_->GetFloatValue(groupName, "InputInterval");
	vehicle_.handling.inputDecrementInterval = globalVariables_->GetFloatValue(groupName, "InputDecrementInterval");
	vehicle_.handling.maxSteerInputCount = globalVariables_->GetIntValue(groupName, "InputMaxCount");
	vehicle_.handling.maxEffectiveCount = globalVariables_->GetIntValue(groupName, "MaxEffectiveCount");

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
	globalVariables_->AddItem(groupName, "WalkSpeed", float(player_.walkSpeedFactor));

	//---カメラ---//
	globalVariables_->AddItem(groupName, "CameraRotateSpeed", float(player_.cameraRotateSpeed));
	// 俯瞰
	groupName = "OverheadCamera";
	overheadCamera_.defaultData.Initialize(groupName, globalVariables_);
	// 運転
	groupName = "DriveCamera";
	driveCamera_.defaultData.Initialize(groupName, globalVariables_);
	globalVariables_->AddItem(groupName, "BackTrackingDelay", float(driveCamera_.backTrackingDelay));

	// 車両
	groupName = "Vehicle";
	globalVariables_->AddItem(groupName, "Core_InitPosition", Vector3(vehicle_.core.initPosition));
	globalVariables_->AddItem(groupName, "Core_LocalPosition", Vector3(vehicle_.core.inVehicleLocal));
	// オーバーヒート
	globalVariables_->AddItem(groupName, "OverheatMaxDamage", float(vehicle_.overheat.maxDPS));
	globalVariables_->AddItem(groupName, "OverheatMinDamage", float(vehicle_.overheat.minDPS));
	globalVariables_->AddItem(groupName, "PushAngleThreshold", float(vehicle_.overheat.pushAngleThreshold));
	globalVariables_->AddItem(groupName, "PushForcePerEngine", float(vehicle_.overheat.pushForcePerEngine));
	globalVariables_->AddItem(groupName, "KnockbackDecayFactor", float(vehicle_.overheat.knockbackDecayFactor));

	// 
	groupName = "VehicleEngine";
	// 入力
	globalVariables_->AddItem(groupName, "InputInterval", float(vehicle_.engine.inputInterval));
	globalVariables_->AddItem(groupName, "InputDecrementInterval", float(vehicle_.engine.inputDecrementInterval));
	globalVariables_->AddItem(groupName, "InputMaxCount", int32_t(vehicle_.engine.maxInputCount));

	// 摩擦
	globalVariables_->AddItem(groupName, "RoadFriction", float(vehicle_.engine.roadFriction));
	globalVariables_->AddItem(groupName, "DirtFriction", float(vehicle_.engine.dirtFriction));
	
	// 加速度
	globalVariables_->AddItem(groupName, "MaxEngineCountAccelFactor", float(vehicle_.engine.maxEngineCountAccelFactor));
	globalVariables_->AddItem(groupName, "MinEngineCountAccelFactor", float(vehicle_.engine.minEngineCountAccelFactor));
	globalVariables_->AddItem(groupName, "AccelerationMultiplier", float(vehicle_.engine.accelerationMultiplier));
	globalVariables_->AddItem(groupName, "IdleDecelerationFactor", float(vehicle_.engine.idleDecelerationFactor));
	globalVariables_->AddItem(groupName, "StopDecelerationFactor", float(vehicle_.engine.stopDecelerationRate));

	// 数・閾値
	globalVariables_->AddItem(groupName, "MaxEffectiveCount", int32_t(vehicle_.engine.maxEffectiveCount));
	globalVariables_->AddItem(groupName, "OverheatEngineCountThreshold", int32_t(vehicle_.engine.overheatEngineThreshold));

	groupName = "VehicleHandling";
	globalVariables_->AddItem(groupName, "SteerMaxAngle", float(vehicle_.handling.steerMaxAngle));
	globalVariables_->AddItem(groupName, "SteerMinAngle", float(vehicle_.handling.steerMinAngle));
	
	globalVariables_->AddItem(groupName, "InputInterval", float(vehicle_.handling.inputInterval));
	globalVariables_->AddItem(groupName, "InputDecrementInterval", float(vehicle_.handling.inputDecrementInterval));
	globalVariables_->AddItem(groupName, "InputMaxCount", int32_t(vehicle_.handling.maxSteerInputCount));
	globalVariables_->AddItem(groupName, "MaxEffectiveCount", int32_t(vehicle_.handling.maxEffectiveCount));

}
