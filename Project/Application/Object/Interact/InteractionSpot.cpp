#include "InteractionSpot.h"

void InteractionSpot::Initialize(LevelData::MeshData* data)
{
	// 初期化
	MeshObject::Initialize(data);
	// マテリアル
	material_->SetEnableLighting(HalfLambert);
	material_->SetColor({ 1,1,1,0 });
	// 方向フラグ
	worldTransform_.usedDirection_ = true;
}

void InteractionSpot::Update()
{
	// 更新
	MeshObject::Update();
	worldTransform_.UpdateMatrix();
}
