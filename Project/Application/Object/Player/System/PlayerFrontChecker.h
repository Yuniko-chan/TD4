#pragma once
#include "../../Utility/Common/OwnerComponent.h"

#include "../../../Engine/Math/Vector/Vector3.h"

class Player;

class PlayerFrontChecker : public OwnerComponent<Player>
{
public:

	void Update();
	bool FrontCheck(const Vector3& direct);
	bool IsInRange(const Vector3& target);
private:
	// 前方ベクトル
	Vector3 frontVector_ = {};
	// 前方か
	bool isFront_ = false;

};

