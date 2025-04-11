#pragma once
#include <memory>
#include "../../../externals/DirectXTex/d3dx12.h"
#include "../../2D/SpriteVertex.h"
#include "../../Math/Matrix/Matrix4x4.h"
#include "../../2D/SpriteForGPU.h"
#include "../../3D/Material/Material.h"
#include "../Texture/TextureUAV.h"

/// <summary>
/// ウインドウスプライト
/// </summary>
class WindowSprite
{

public: // メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static WindowSprite* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="commandList">コマンドリスト</param>
	void Initialize(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画SRV
	/// </summary>
	/// <param name="srvGPUHandle">GPUハンドル</param>
	void DrawSRV(const CD3DX12_GPU_DESCRIPTOR_HANDLE& srvGPUHandle);

	/// <summary>
	/// 描画SRV
	/// </summary>
	/// <param name="textureUAV">テクスチャ</param>
	void DrawSRV(TextureUAV* textureUAV);

private: // メンバ変数、定数

	// 頂点数
	const int kVertNum_ = 3;
	// デバイス
	ID3D12Device* device_;
	// コマンドリスト
	ID3D12GraphicsCommandList* commandList_;


};

