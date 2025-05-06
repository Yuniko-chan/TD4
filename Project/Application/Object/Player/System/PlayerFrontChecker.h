#pragma once
#include "PlayerComponent.h"

#include "../../../Engine/Math/Vector/Vector3.h"

class PlayerFrontChecker : public PlayerComponent
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

