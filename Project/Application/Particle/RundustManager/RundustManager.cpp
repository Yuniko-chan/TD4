#include "RunDustManager.h"

void RunDustManager::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();

	// パーティクル
	runDustParticle_ = std::make_unique<RunDustParticle>();
	runDustParticle_->Initialize(
		dxCommon_->GetDevice(),
		dxCommon_->GetCommadListLoad(),
		GraphicsPipelineState::sRootSignature_[GraphicsPipelineState::kPipelineStateIndexGPUParticleBlendNormal].Get(),
		GraphicsPipelineState::sPipelineState_[GraphicsPipelineState::kPipelineStateIndexGPUParticleBlendNormal].Get());

	// 発生位置
	emitPositions_ = {};

	// 現在の発生数
	emitNum_ = 0;

}

void RunDustManager::Update()
{

	RunDustParticle::EmitBlendNormalCS emitter;


	// 生成しない
	emitter.count = 30;
	emitter.frequency = 1.0f;
	emitter.radius = 1.0f;
	emitter.num = emitNum_;

	emitter.frequencyTime = 1.0f;
	emitter.translate0 = emitPositions_[0];
	emitter.translate1 = emitPositions_[1];
	emitter.translate2 = emitPositions_[2];
	emitter.translate3 = emitPositions_[3];
	emitter.emit = 0;

	runDustParticle_->SetEmitter(emitter, true);


	runDustParticle_->Update();

}

void RunDustManager::Draw(BaseCamera& camera)
{

	runDustParticle_->Draw(dxCommon_->GetCommadList(), camera);

}

void RunDustManager::PositionRegister(const Vector3& position)
{

	if (emitNum_ == kEmitNumMax) {
		return;
	}

	emitPositions_[emitNum_] = position;
	// 追加分
	emitPositions_[emitNum_].y += 3.0f;

	emitNum_++;

}

void RunDustManager::PositionClear()
{

	for (uint32_t i = 0; i < kEmitNumMax; ++i) {
		emitPositions_[i] = { 0.0f,0.0f,0.0f };
	}

	emitNum_ = 0;

}
