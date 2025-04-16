#include "PartsInterface.h"

void Car::IParts::Initialize(LevelData::MeshData* data)
{
	// 基底
	MeshObject::Initialize(data);
	material_->SetEnableLighting(HalfLambert);

}

void Car::IParts::Draw(BaseCamera& camera)
{
	camera;
}
