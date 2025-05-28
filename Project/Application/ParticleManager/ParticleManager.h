#pragma once
#include "../../../Engine/GPUParticle/GPUParticle.h"
#include "../../../Engine/Scene/BaseScene/BaseScene.h"

#include "../Particle/RunDustParticle/RunDustParticle.h"

class ParticleManager {
public:
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

		indexList_[particleName] = number;

		particles_[indexList_[particleName]].reset(result);
		particles_[indexList_[particleName]]->Initialize(
			dxCommon_->GetDevice(),
			dxCommon_->GetCommadListLoad(),
			GraphicsPipelineState::sRootSignature_[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get(),
			GraphicsPipelineState::sPipelineState_[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get(), particleName);
		particles_[indexList_[particleName]]->SetEmitter(kEmitter,true);

		number++;
	}
private:
	std::array<std::unique_ptr<GPUParticle>, ParticleIndex::kUIIndexOfCount> particles_;

	std::unordered_map<std::string, int> indexList_;
	int number = 0;
};
