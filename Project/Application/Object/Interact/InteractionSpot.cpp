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
	isDraw_ = false;
}

void InteractionSpot::Update()
{
	// フラグがfalseなら表示しない
	if (!isDraw_) {
		Vector4 color = Vector4(color_.x, color_.y, color_.z, 0.0f);
		material_->SetColor(color);
	}
	else {
		Vector4 color = Vector4(color_.x, color_.y, color_.z, alpha_);
		material_->SetColor(color);
	}

	// 更新
	MeshObject::Update();
	worldTransform_.UpdateMatrix();
}
