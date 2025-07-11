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
	void OnHit(float damage);

public:
	void SetHP(int16_t hp) { hp_ = (float)hp; }

	/// <summary>
	/// 無敵経過
	/// </summary>
	void InvisibleProgress();

	void HeatDamage(float damage = 1.0f);

	float GetHP() { return hp_; }
	// 爆発処理用
	bool IsDead();
private:
	
	int16_t maxHP_ = 0;
	float hp_ = 0;
	bool isInvisible_ = false;
	bool isDead_ = false;
	float invisibleCooltime_ = 0.0f;

};
