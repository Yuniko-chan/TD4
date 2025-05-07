#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"

/// <summary>
/// プロト用の静的変数パラメータを管理しておくクラス
/// </summary>
class PlayerDebugData
{
public:	// 構造体
	struct RideActionData
	{
		float jumpHeight = 20.0f;
		float actionFrame = 60.0f;
		Vector3 offset = { 0.0f,1.95f,0.0f };

		void ImGuiDraw(const std::string& name, float dragValue);
	};
	struct MoveParamData
	{
		// 車両の速度
		float rideSpeed = 2.0f;
		// 移動速度
		float moveSpeed = 1.0f;
		// 回転係数
		float rotateRatio = 1.0f / 45.0f;

		void ImGuiDraw(const std::string& name, float dragValue);
	};

	struct FrontCheckerData
	{
		float frontThreshold = 0.8f;
		float catchRange = 50.0f;
	};

public:	// 静的変数
	// 乗り込むアクションのデータ
	static RideActionData sRideActionData;
	static RideActionData sDropoffActionData;
	// 移動の速度
	static MoveParamData sMoveData;
	
	static float sFrontThreshold;
	static FrontCheckerData sFrontCheckData;
public:
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

};