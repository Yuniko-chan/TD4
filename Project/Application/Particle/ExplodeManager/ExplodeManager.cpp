#include "ExplodeManager.h"

void ExplodeManager::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();

	// パーティクル
	ExplodeParticle_ = std::make_unique<ExplodeParticle>();
	ExplodeParticle_->Initialize(
		dxCommon_->GetDevice(),
		dxCommon_->GetCommadListLoad(),
		GraphicsPipelineState::sRootSignature_[GraphicsPipelineState::kPipelineStateIndexGPUParticleBlendNormal].Get(),
		GraphicsPipelineState::sPipelineState_[GraphicsPipelineState::kPipelineStateIndexGPUParticleBlendNormal].Get());

	// 発生位置
	emitPositions_ = {};

	// 現在の発生数
	emitNum_ = 0;

}

void ExplodeManager::Update()
{

	ExplodeParticle::EmitBlendNormalCS emitter;


	// 生成しない
	if (emitNum_ == 0) {
		emitter.frequencyTime = 0.0f;
		emitter.translate0 = { 0.0f,0.0f,0.0f };
		emitter.translate1 = { 0.0f,0.0f,0.0f };
		emitter.translate2 = { 0.0f,0.0f,0.0f };
		emitter.translate3 = { 0.0f,0.0f,0.0f };

		emitter.count = 0;
		emitter.frequency = 1.0f;
		emitter.radius = 1.0f;
		emitter.num = emitNum_;

		emitter.emit = 0;
		ExplodeParticle_->SetEmitter(emitter, true);
	}
	else {
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

		ExplodeParticle_->SetEmitter(emitter, true);

	}

	ExplodeParticle_->Update();

}

void ExplodeManager::Draw(BaseCamera& camera)
{

	ExplodeParticle_->Draw(dxCommon_->GetCommadList(), camera);

}

void ExplodeManager::PositionRegister(const Vector3& position)
{

	if (emitNum_ == kEmitNumMax) {
		return;
	}

	emitPositions_[emitNum_] = position;
	// 追加分
	emitPositions_[emitNum_].y += 3.0f;

	emitNum_++;

}

void ExplodeManager::PositionClear()
{

	for (uint32_t i = 0; i < kEmitNumMax; ++i) {
		emitPositions_[i] = { 0.0f,0.0f,0.0f };
	}

	emitNum_ = 0;

}
