#pragma once
#include "../../../Utility/Common/OwnerComponent.h"

#include <cstdint>

namespace Car { class IParts; }

class PartHPHandler :
	public OwnerComponent<Car::IParts>
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

	/// <summary>
	/// 被弾
	/// </summary>
	/// <param name="damage"></param>
	void OnHit(int16_t damage);

public:
	void SetHP(int16_t hp) { hp_ = hp; }

private:
	
	int16_t hp_ = 0;
	bool isInvisible_ = false;
};
