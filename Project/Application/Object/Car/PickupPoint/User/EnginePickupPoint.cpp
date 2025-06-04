#include "EnginePickupPoint.h"

void EnginePickupPoint::Initialize(LevelData::MeshData* data)
{
	// 初期化
	IPickupPoint::Initialize(data);
	//worldTransform_.transform_.translate = { 0.0f,0.0f,20.0f };

	type_ = kEngine;
}
