#pragma once
#include "../../../Utility/Common/OwnerComponent.h"
#include "../../../../../Engine/Math/Vector/Vector4.h"

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
	void OnAttach();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 被弾
	/// </summary>
	/// <param name="damage"></param>
	void OnHit(float damage);

	void Setup(int16_t maxHP = 20);

public:
	void SetHP(int16_t hp) { hp_ = (float)hp; }

	/// <summary>
	/// 無敵経過
	/// </summary>
	void InvisibleProgress();

	void HeatDamage(float damage = 1.0f);

	float GetHP() { return hp_; }
	/// <summary>
	/// IsDelete && HPがなくなった時のIsDead
	/// </summary>
	/// <returns></returns>
	bool IsDead();

	/// <summary>
	/// 解除処理（リストから
	/// </summary>
	/// <returns></returns>
	bool IsUnRegist() { return isUnregist_; }

	/// <summary>
	/// 持たれてる
	/// </summary>
	void BeingHeld();

private:
	Vector4 defaultColor_ = Vector4();
	Vector4 damageColor_ = Vector4();
	int16_t maxHP_ = 0;
	float hp_ = 0;
	bool isInvisible_ = false;
	bool isDead_ = false;
	bool isUnregist_ = false;
	float invisibleCooltime_ = 0.0f;

	bool beingHeld_ = false;

};
