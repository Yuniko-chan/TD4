#include "DustParticle.h"

void DustParticle::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState, const std::string& name)
{


	// おかしな呼び方をされていないか
	if (name != "Particle") {
		assert(0);
	}

	// テクスチャ名前
	const std::string kTextureFilename = "Dust0.png";
	textureFilename_ = kTextureFilename;

	// 初期化
	const std::string kName = "Dust";
	GPUParticle::Initialize(device, commandList, rootSignature, pipelineState, kName);

	// エミッタ設定
	const EmitterCS kEmitter =
	{
			Vector3{0.0f, 3.0f, 0.0f}, // 位置
			0.5f, // 射出半径
			30, // 射出数
			0.05f, // 射出間隔
			0.0f, // 射出間隔調整時間
			0 // 射出許可
	};
	SetEmitter(kEmitter);

}
