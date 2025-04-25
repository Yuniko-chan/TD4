#include "CourseCollisionPipeline.h"
#include "../../Engine/base/CompileShader.h"
#include "../../Engine/base/DxCommon/Log.h"

void CourseCollisionPipeline::Initialize(ID3D12Device* device)
{

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	// SRV
	D3D12_DESCRIPTOR_RANGE descriptorRangeSRV[1] = {};
	descriptorRangeSRV[0].BaseShaderRegister = 0;//0から始まる
	descriptorRangeSRV[0].NumDescriptors = 1;//数は一つ
	descriptorRangeSRV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;// SRVを使う
	descriptorRangeSRV[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	// UAV
	D3D12_DESCRIPTOR_RANGE descriptorRangeUAV[1] = {};
	descriptorRangeUAV[0].BaseShaderRegister = 1;//0から始まる
	descriptorRangeUAV[0].NumDescriptors = 1;//数は一つ
	descriptorRangeUAV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;// UAVを使う
	descriptorRangeUAV[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeSRV;//Tableの中身の配列を指定
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeSRV);//Tableで利用する数

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRangeUAV;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeUAV);//Tableで利用する数

	// 共通関数に送る
	PipelineStateCSCommonInitialize(
		device,
		rootParameters,
		_countof(rootParameters),
		L"Resources/Shaders/Cource/CourceExtrusion.CS.hlsl");

}

void CourseCollisionPipeline::SetPipelineState(ID3D12GraphicsCommandList* commandList)
{

	commandList->SetPipelineState(pipelineState_.Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignature_.Get());

}

void CourseCollisionPipeline::PipelineStateCSCommonInitialize(ID3D12Device* device, D3D12_ROOT_PARAMETER* rootParameters, uint32_t numParameters, const std::wstring& filePath)
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	descriptionRootsignature.pParameters = rootParameters; //ルートパラメータ配列へのポインタ
	descriptionRootsignature.NumParameters = numParameters; //配列の長さ

	// サンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
	samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].MipLODBias = 0.0f;
	samplerDesc[0].MaxAnisotropy = 0;
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc[0].MinLOD = 0.0f;
	samplerDesc[0].MaxLOD = 3.402823466e+38f;
	samplerDesc[0].RegisterSpace = 0;
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	descriptionRootsignature.pStaticSamplers = samplerDesc;
	descriptionRootsignature.NumStaticSamplers = _countof(samplerDesc);

	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootsignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log::Message(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		filePath,
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignature_.Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(hr));

}
