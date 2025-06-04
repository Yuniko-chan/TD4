#pragma once
#include <cstdint>
#include "../../../../Collider/ColliderParentObject.h"

// ギミック
class ConicalPendulumIronBall;
class PendulumIronBall;
class CannonBall;
class CannonExplosion;
class MinigunBullet;
class Obstacle;

// パーツ
namespace Car { class IParts; }

/// <summary>
/// 衝突管理クラス
/// </summary>
class PartCollisionHandler
{

public: // サブクラス

	// variantでまとめる
	using GimmickObject = std::variant<
		ConicalPendulumIronBall*, PendulumIronBall*, 
		CannonBall*, CannonExplosion*,
		MinigunBullet*, Obstacle*>;

public: // 関数

	/// <summary>
	///	初期化
	/// </summary>
	static void Initialize();

	static void Execute(Car::IParts* part, ColliderParentObject colliderPartner);

private: // 関数

	/// <summary>
	/// ギミックか
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <returns></returns>
	static GimmickObject CheckGimmick(ColliderParentObject colliderPartner);

	/// <summary>
	/// ギミック
	/// </summary>
	/// <param name="part">パーツ</param>
	/// <param name="colliderPartner"></param>
	static void OnCollisionGimmick(Car::IParts* part, GimmickObject colliderPartner);

};

