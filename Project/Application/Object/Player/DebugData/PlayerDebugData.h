#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"

/// <summary>
/// プロト用の静的変数パラメータを管理しておくクラス
/// </summary>
class PlayerDebugData
{
public:
	struct RideActionData
	{
		float jumpHeight = 20.0f;
		float actionFrame = 60.0f;
		Vector3 offset = { 0.0f,3.0f,0.0f };

		void ImGuiDraw(const std::string& name, float dragValue);
	};

	// 乗り込むアクションのデータ
	static RideActionData sRideActionData;
	static RideActionData sDropoffActionData;

public:
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

};