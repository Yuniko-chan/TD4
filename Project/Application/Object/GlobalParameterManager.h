#pragma once
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "Utility/Common/SingletonModule.h"
#include "Utility/GlobalParameterStructs.h"

class GlobalParameterManager : public SingletonModule<GlobalParameterManager>
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
private:
	/// <summary>
	/// 値の更新
	/// </summary>
	void ApplyGlobalVariables();
	/// <summary>
	/// アイテムの追加処理まとめ
	/// </summary>
	void AddItems();

private:
	GlobalVariables* globalVariables_ = nullptr;
	// プレイヤー
	ParameterStructs::PlayerData player_ = {};
	// 車両関係
	ParameterStructs::VehicleData vehicle_ = {};

	//---カメラ---//
	// 共通データ
	ParameterStructs::CameraCommonData cameraCommon_ = {};

	// それぞれ
	ParameterStructs::OverheadCameraData overheadCamera_ = {};
	ParameterStructs::DriveCameraData driveCamera_ = {};
};
