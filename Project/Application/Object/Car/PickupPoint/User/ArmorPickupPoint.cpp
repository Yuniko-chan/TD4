#include "ArmorPickupPoint.h"

void ArmorPickupPoint::Initialize(LevelData::MeshData* data)
{
	// 初期化
	IPickupPoint::Initialize(data);

	type_ = kArmor;
}
