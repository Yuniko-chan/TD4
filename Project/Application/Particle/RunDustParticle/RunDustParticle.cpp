#include "RunDustParticle.h"

void RunDustParticle::Initialize(
	ID3D12Device* device, 
	ID3D12GraphicsCommandList* commandList, 
	ID3D12RootSignature* rootSignature, 
	ID3D12PipelineState* pipelineState,
	const std::string& name)
{
	// テクスチャ名前
	const std::string kTextureFilename = "circle.png";
	textureFilename_ = kTextureFilename;

	// 初期化
	GPUParticle::Initialize(device, commandList, rootSignature, pipelineState, name);
}
