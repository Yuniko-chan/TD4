#pragma once
#include "../../Utility/Common/OwnerComponent.h"

#include "../../../Engine/Math/Vector/Vector3.h"

class Player;

class PlayerFrontChecker : public OwnerComponent<Player>
{
public:
	/// <summary>
	/// 向きなどの更新
	/// </summary>
	void Update();
	/// <summary>
	/// 前方チェック
	/// </summary>
	/// <param name="direct"></param>
	/// <returns></returns>
	bool FrontCheck(const Vector3& direct);
	/// <summary>
	/// 範囲内か（パーツ編）
	/// </summary>
	/// <param name="target"></param>
	/// <returns></returns>
	bool IsInRange(const Vector3& target);
	/// <summary>
	/// 回収ポイントでの範囲チェック
	/// </summary>
	/// <param name="target"></param>
	/// <returns></returns>
	bool IsPointInRange(const Vector3& target);
private:
	// 前方ベクトル
	Vector3 frontVector_ = {};
	// 前方か
	bool isFront_ = false;

};

