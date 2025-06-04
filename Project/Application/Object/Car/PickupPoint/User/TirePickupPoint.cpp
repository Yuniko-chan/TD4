#include "TirePickupPoint.h"

void TirePickupPoint::Initialize(LevelData::MeshData* data)
{
	// 初期化
	IPickupPoint::Initialize(data);
	//worldTransform_.transform_.translate = { -20.0f,0.0f,0.0f };

	type_ = kTire;
}
