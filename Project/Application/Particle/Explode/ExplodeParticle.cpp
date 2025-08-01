#include "ExplodeParticle.h"

void ExplodeParticle::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState, const std::string& name)
{
	textureFilename_ = "circle.png";

	GPUParticle3D::Initialize(device, commandList, rootSignature, pipelineState, name);

	Model_ = ModelManager::GetInstance()->GetModel("Resources/Model/Engine/", "Engine.obj");
}
