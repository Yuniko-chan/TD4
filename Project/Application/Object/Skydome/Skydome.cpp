#include "Skydome.h"
#include <cassert>
#include "../../../Engine/2D/ImguiManager.h"
#include <numbers>
#include "../../../Engine/3D/Model/ModelDraw.h"

void Skydome::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	material_->SetEnableLighting(BlinnPhongReflection);

}

void Skydome::Update() 
{

	// 速度
	const float kSpeed = 0.001f;
	worldTransform_.transform_.rotate.y = fmodf(worldTransform_.transform_.rotate.y + kSpeed, static_cast<float>(std::numbers::pi) * 2.0f);

	worldTransform_.UpdateMatrix();
	
}
