#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include <string>
#include <list>

namespace ParameterStructs {

	namespace Datas
	{
		struct RideAction {
			float jumpHeight = 0.0f;
			float actionFrame = 0.0f;
			Vector3 offset = {};
		};

		struct FrontCheck
		{
			float threshold = 0.0f;	// 前方の閾値
			float catchRange = 0.0f;	// 拾う距離
		};

	}

	struct StringData {
		std::string groupName;
		std::list<std::string> keys;
	};

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

	};



}
