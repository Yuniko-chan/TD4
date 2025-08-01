#pragma once
#include "../../../Engine/GPUParticle/GPUParticle.h"
#include "../../../Engine/Scene/BaseScene/BaseScene.h"

#include "../Particle/RunDustParticle/RunDustParticle.h"
#include "../Particle/Explode/ExplodeParticle.h"
class ParticleManager {
public:
	struct ParticleData {
		int indexNumber = 0;
		bool active = true;
	};
	// テクスチャ一覧
	enum TextureIndex {
		kTextureIndexOfCount
	};

	// パーティクル一覧
	enum ParticleIndex {
		RunDust,
		//GPU,
		kUIIndexOfCount
	};

public:
	/// <summary>
	///	初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(BaseCamera& camera);
	/// <summary>
	/// パーティクルの生成を止める
	/// </summary>
	/// <param name="particleName">生成時に付けた名前</param>
	void StopEmission(std::string particleName);
	/// <summary>
	/// パーティクルの生成を止める
	/// </summary>
	/// <param name="particleName">生成時に付けた名前</param>
	void StertEmission(std::string particleName);

	/// <summary>
	/// Imgui描画
	/// </summary>
	void ImGuiDraw();
	template <typename T>
	void CreateParticle(std::string particleName, 
		GPUParticle3D::EmitBlendNormalCS kEmitter)
{
		kEmitter.count = 30;
		kEmitter.frequency = 1.0f;
		kEmitter.radius = 1.0f;
		kEmitter.num = 10;

		kEmitter.frequencyTime = 1.0f;
		kEmitter.translate0 = {0.0f,0.0f,1.0f};
		kEmitter.translate1 = {0.0f,0.0f,1.0f};
		kEmitter.translate2 = {0.0f,0.0f,1.0f};
		kEmitter.translate3 = {0.0f,0.0f,1.0f};
		kEmitter.emit = true;

		GPUParticle3D* result = new T;

		// DirectXCommon
		DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();

		particleInfo_[particleName].indexNumber = number;

		particles_[particleInfo_[particleName].indexNumber].reset(result);
		particles_[particleInfo_[particleName].indexNumber]->Initialize(
			dxCommon_->GetDevice(),
			dxCommon_->GetCommadListLoad(),
			GraphicsPipelineState::sRootSignature_[GraphicsPipelineState::kPipelineStateIndexGPUParticleBlendNormal].Get(),
			GraphicsPipelineState::sPipelineState_[GraphicsPipelineState::kPipelineStateIndexGPUParticleBlendNormal].Get(), particleName);
		particles_[particleInfo_[particleName].indexNumber]->SetEmitter(kEmitter,true);

		number++;
	}
private:
	std::array<std::unique_ptr<GPUParticle3D>, ParticleIndex::kUIIndexOfCount> particles_;

	std::unordered_map<std::string, ParticleData> particleInfo_;
	int number = 0;
};
