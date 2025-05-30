#include "EnginePickupPoint.h"

void EnginePickupPoint::Initialize(LevelData::MeshData* data)
{
	// 初期化
	IPickupPoint::Initialize(data);

	type_ = kEngine;
}
