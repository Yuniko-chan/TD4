#include "InterfacePickupPoint.h"
#include "../../../Collider/CollisionConfig.h"

#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"

void IPickupPoint::Initialize(LevelData::MeshData* data)
{
	// 基底
	MeshObject::Initialize(data);
	material_->SetEnableLighting(HalfLambert);
	// 方向フラグ
	worldTransform_.usedDirection_ = true;

	// 衝突マスク
	collisionAttribute_ = kCollisionAttributeVehicleParts_;
	collisionMask_ -= kCollisionAttributeVehicleParts_;
}

void IPickupPoint::Update()
{
	// メッシュの更新
	MeshObject::Update();

}
