#include "ParticleManager.h"

void ParticleManager::Initialize()
{

}

void ParticleManager::Update()
{
	for (uint32_t i = 0; i < ParticleIndex::kUIIndexOfCount; ++i) {
		particles_[i]->Update();
	}
}

void ParticleManager::Draw(BaseCamera& camera)
{
	for (uint32_t i = 0; i < ParticleIndex::kUIIndexOfCount; ++i) {
		particles_[i]->Draw(DirectXCommon::GetInstance()->GetCommadList(), camera);
	}
}

void ParticleManager::ImGuiDraw()
{

}