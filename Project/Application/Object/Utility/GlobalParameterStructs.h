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

		// コア
		struct VehicleCore
		{
			Vector3 initPosition = {};
			Vector3 inVehicleLocal = {};
		};

		struct VehicleParts
		{
			int engineHP = 10;
			int tireHP = 10;
			int armorHP = 10;

			// ピックアップ
			Vector3 pickupPredictionScale = {};	// サイズ
			Vector3 pickupColor = {};	// 色
			float pickupAlpha = 0.0f;	// アルファ値
			Vector3 pickupPointPredictionScale = {};	// ピックアップポイント用のサイズ

			// アタッチ
			Vector3 attachPredictionScale = {};	// サイズ
			Vector3 attachColor = {};	// 色
			float attachAlpha = 0.0f;	// アルファ値
		};

		// ハンドル関係
		struct VehicleHandling
		{
			// ハンドルの最大角度
			float steerMaxAngle = 2.0f;
			// ハンドルの最小角度
			float steerMinAngle = 0.1f;
			// 入力間隔
			float inputInterval = 5.0f;
			// 減少の間隔
			float inputDecrementInterval = 2.0f;

			float tireLerpMin = 0.01f;
			float tireLerpMax = 1.0f;

			// ステアの切り返しの大きいかを判定する閾値
			int steerReturnAccelThreshold = 0;
			// 切り返しの大きい際のセンシ
			int highInputReturnSensitivity = 0;
			// ハンドル入力の最大値
			int32_t maxSteerInputCount = 0;

			// 影響の出る最大値
			int maxEffectiveCount = 10;
		};

		// エンジン関係
		struct VehicleEngine 
		{
			// 入力間隔
			float inputInterval = 5.0f;
			// 減少の間隔
			float inputDecrementInterval = 2.0f;
			// 受付の最大値
			int32_t maxInputCount = 0;

			// 摩擦
			float dirtFriction = 0.55f;
			float roadFriction = 0.75f;
			// 影響の出る最大値
			int maxEffectiveCount = 10;

			// 加速度
			float maxEngineCountAccelFactor = 10.0f;
			float minEngineCountAccelFactor = 1.0f;
			float accelerationMultiplier = 1.0f;	// デフォルト乗算係数

			float idleDecelerationFactor = 0.05f;
			float stopDecelerationRate = 0.1f;
			// オーバーヒートするかのエンジン数閾値
			int overheatEngineThreshold = 0;
		};

		// オーバーヒート
		struct Overheat
		{
			float maxDPS = 7.0f;	// 最小
			float minDPS = 1.0f;	// 最大

			float pushAngleThreshold = 0.0f;
			float pushForcePerEngine = 0.0f;
			float knockbackDecayFactor = 0.15f;
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
		// ハンドル
		Datas::VehicleHandling handling;
		// エンジン
		Datas::VehicleEngine engine;
		// コア
		Datas::VehicleCore core;
		// オーバーヒート関係
		Datas::Overheat overheat;
		// パーツごと
		Datas::VehicleParts parts;

		// 加速度の丸める値
		float accelDiscard = 0.75f;

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
		// バックするときのディレイ
		float backTrackingDelay = 0.0f;
	};

	struct CameraCommonData {
		// Pair<Offset,Rotation>
		std::pair<Vector3, Vector3> inVehicle;
		std::pair<Vector3, Vector3> onFoot;
	};

	/// <summary>
	/// ゲームのシステムデータ
	/// </summary>
	struct GameSystemData {
		// 制限時間
		float limitSecond = 300.0f;
		// 追加秒数
		float checkpointAddSecond = 15.0f;
	};

}
