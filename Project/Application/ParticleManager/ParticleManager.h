#pragma once
#include "../../../Engine/GPUParticle/GPUParticle.h"
#include "../../../Engine/Scene/BaseScene/BaseScene.h"

#include "../Particle/RunDustParticle/RunDustParticle.h"
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
		GPU,
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
	/// Imgui描画
	/// </summary>
	void ImGuiDraw();
	template <typename T>
	void CreateParticle(std::string particleName, 
		EmitterCS kEmitter =
		{
			{ 0.0f,0.0f,0.0f},	// 位置
			1.0f,				// 射出半径
			10,					// 射出数
			0.1f,				// 射出間隔
			0.0f,				// 射出間隔調整時間
			true				// 射出許可
		}) 
{
		GPUParticle* result = new T;

		// DirectXCommon
		DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();

		particleInfo_[particleName].indexNumber = number;

		particles_[particleInfo_[particleName].indexNumber].reset(result);
		particles_[particleInfo_[particleName].indexNumber]->Initialize(
			dxCommon_->GetDevice(),
			dxCommon_->GetCommadListLoad(),
			GraphicsPipelineState::sRootSignature_[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get(),
			GraphicsPipelineState::sPipelineState_[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get(), particleName);
		particles_[particleInfo_[particleName].indexNumber]->SetEmitter(kEmitter,true);

		number++;
	}
private:
	std::array<std::unique_ptr<GPUParticle>, ParticleIndex::kUIIndexOfCount> particles_;

	std::unordered_map<std::string, ParticleData> particleInfo_;
	int number = 0;
};
