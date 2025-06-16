#pragma once
#include "Model.h"

/// <summary>
/// インスタンシング描画データ
/// </summary>
struct InstancingDrawingData {

	// モデル
	Model* model;
	// アニメーションするか
	bool isAnimation;

	// トランスフォーム SRV
	Microsoft::WRL::ComPtr<ID3D12Resource> transformBuff;
	// トランスフォーム マップ
	TransformationMatrix* transformMap = nullptr;
	// トランスフォーム CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE transformSrvHandleCPU{};
	// トランスフォーム GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE transformSrvHandleGPU{};
	// トランスフォーム ディスクリプタヒープの位置
	uint32_t transformSrvIndexDescriptorHeap = 0;

	// マテリアル SRV
	Microsoft::WRL::ComPtr<ID3D12Resource> materialBuff;
	// マテリアル マップ
	SRVMaterialData* materialMap = nullptr;
	// マテリアル CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE materialSrvHandleCPU{};
	// マテリアル GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE materialSrvHandleGPU{};
	// マテリアル ディスクリプタヒープの位置
	uint32_t materialSrvIndexDescriptorHeap = 0;

};
