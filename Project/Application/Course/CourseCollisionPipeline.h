#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <array>

#pragma comment(lib, "dxcompiler.lib")

/// <summary>
/// コース衝突パイプライン
/// </summary>
class CourseCollisionPipeline
{

private: // 変数

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// パイプラインステート設定
	/// </summary>
	/// <param name="commandList"></param>
	/// <param name="index"></param>
	void SetPipelineState(ID3D12GraphicsCommandList* commandList);

private: // 関数

	/// <summary>
	/// パイプラインステートの初期化 共通関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="rootParameters">ルートパラメータ</param>
	/// <param name="numParameters">ルートパラメータ数</param>
	/// <param name="filePath">HLSLのファイルパス</param>
	void PipelineStateCSCommonInitialize(
		ID3D12Device* device,
		D3D12_ROOT_PARAMETER* rootParameters,
		uint32_t numParameters,
		const std::wstring& filePath);


};

