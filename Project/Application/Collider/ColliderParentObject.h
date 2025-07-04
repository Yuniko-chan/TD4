#pragma once
#include <variant>
#include "../../Engine/Collision/CollisionData.h"

/// <summary>
/// 衝突時に「OnCollision」を呼ぶための設定
/// </summary>

// 前方宣言
class ParentNullObject;
// 
class Player;
// 車両パーツ
class VehicleCore;
class TireParts;
class ArmorFrameParts;
class EngineParts;
class TerrainObject;

// ギミック
class ConicalPendulumIronBall;
class PendulumIronBall;
class Cannon;
class CannonBall;
class CannonExplosion;
class Minigun;
class MinigunBullet;
class Obstacle;

//フィールド
class Wall;
class CustomArea;

// variantでまとめる
using ColliderParentObject = std::variant<ParentNullObject*, Player*, VehicleCore*, 
	ConicalPendulumIronBall*, PendulumIronBall*, Cannon*, CannonBall*, CannonExplosion*, 
	Minigun*, MinigunBullet*, Obstacle*, TerrainObject*
	, TireParts*, ArmorFrameParts*, EngineParts*, Wall*,CustomArea* > ;

/// <summary>
/// 親がないもの用
/// </summary>
class ParentNullObject {

public:

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {
		// 警告回避
		colliderPartner;
		collisionData;
	}

};
