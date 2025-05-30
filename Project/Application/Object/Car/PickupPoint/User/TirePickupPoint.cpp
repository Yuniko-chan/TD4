#include "TirePickupPoint.h"

void TirePickupPoint::Initialize(LevelData::MeshData* data)
{
	// 初期化
	IPickupPoint::Initialize(data);

	type_ = kTire;
}
