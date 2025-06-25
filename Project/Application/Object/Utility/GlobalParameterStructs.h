#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"

#include <string>
#include <list>
#include <algorithm>

class GlobalVariables;

namespace ParameterStructs {

	namespace Datas
	{
		// 乗り込みアクション
		struct RideAction {
			float jumpHeight = 0.0f;
			float actionFrame = 0.0f;
			Vector3 offset = {};
		};
		// 前方確認
		struct FrontCheck
		{
			float threshold = 0.0f;	// 前方の閾値
			float catchRange = 0.0f;	// 拾う距離
		};

		struct VehicleHandling
		{
			// ハンドルの回転の最大値（X）
			float steerMaxDirect = 2.0f;
			// ハンドル入力の間隔
			int handleInputDuration = 6;
		};

		struct VehicleEngine 
		{
			// 加算間隔
			int addDuration = 10;
			// 最大受付数
			int maxReception = 15;
			// 受付数に対する倍率
			float receptionRatio = 3.0f;
		};

		struct CameraDefault {
			Vector3 position = {};
			Vector3 rotateVector = Vector3(0.0f, 0.0f, 1.0f);
			float trackingDelay = 0.1f;

			void Initialize(const char* groupName, GlobalVariables* globalVariables);
			void ApplyGlobalVariables(const char* groupName, GlobalVariables* globalVariables);
		};
	}

	struct StringData {
		std::string groupName;
		std::list<std::string> keys;
	};

	/// <summary>
	/// プレイヤー関係のデータ
	/// </summary>
	struct PlayerData
	{
		// 前方チェック
		Datas::FrontCheck frontCheck;
		// 乗る
		Datas::RideAction ride;
		// 降りる
		Datas::RideAction dropOff;

		// 乗車時の速度
		float rideSpeedFactor = 0.0f;
		// 歩きの速度
		float walkSpeedFactor = 0.0f;
		// 走りの速度
		float dashSpeedFactor = 0.0f;
		// 割り算
		float cameraRotateSpeed = 30.0f;
		
		// パーツ持った時のオフセット
		Vector3 holdOffset = Vector3(0.0f, 0.0f, 2.0f);
	};

	/// <summary>
	/// 車両関係のデータ
	/// </summary>
	struct VehicleData {
		Datas::VehicleHandling handling;
		Datas::VehicleEngine engine;
	};

	/// <summary>
	/// 見下ろしカメラ
	/// </summary>
	struct OverheadCameraData {
		Datas::CameraDefault defaultData;

	};

	/// <summary>
	/// 運転中のカメラ
	/// </summary>
	struct DriveCameraData {
		Datas::CameraDefault defaultData;
	};

	struct CameraCommonData {
		// Pair<Offset,Rotation>
		std::pair<Vector3, Vector3> inVehicle;
		std::pair<Vector3, Vector3> onFoot;
	};

}
