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
	Microsoft::WRL::ComPtr<ID3D12Resource> buff;
	// マップ
	TransformationMatrix* map = nullptr;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU{};
	// ディスクリプタヒープの位置
	uint32_t srvIndexDescriptorHeap = 0;

};
