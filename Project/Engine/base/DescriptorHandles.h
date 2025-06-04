#pragma once
#include <d3d12.h>
#include <cstdint>

/// <summary>
/// ディスクリプタハンドル
/// </summary>
struct DescriptorHandles
{
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t descriptorHeap = 0;

};