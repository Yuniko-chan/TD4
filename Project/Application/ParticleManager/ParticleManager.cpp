#include "ParticleManager.h"

void ParticleManager::Initialize()
{

}

void ParticleManager::Update()
{
	auto it = particles_.begin();
	for (; it != particles_.end(); it++) {
		it->get()->Update();
	}
}

void ParticleManager::Draw(BaseCamera& camera)
{
	for (uint32_t i = 0; i < ParticleIndex::kUIIndexOfCount; ++i) {
		particles_[i]->Draw(DirectXCommon::GetInstance()->GetCommadList(), camera);
	}
}

void ParticleManager::StopEmission(std::string particleName)
{
	// キーが存在するかを確認してからアクセスする
	auto it = particleInfo_.find(particleName);
	if (it != particleInfo_.end()) {
		particleInfo_[particleName].active = false;
		particles_[particleInfo_[particleName].indexNumber]->ChangeEmission(false);
	}
	else {
		std::cout << particleName << " は見つかりませんでした。" << std::endl;
	}
}

void ParticleManager::ImGuiDraw()
{

}