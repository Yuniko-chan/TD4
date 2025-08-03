#include "ExplodeParticle.h"

void ExplodeParticle::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState,
	const std::string& name)
{
	name;
	textureFilename_ = "circle.png";

	GPUParticle::Initialize(device, commandList, rootSignature, pipelineState);

	eggModel_ = ModelManager::GetInstance()->GetModel("Resources/Model/Box/", "Box.obj");

}

void ExplodeParticle::Update()
{

	// 時間加算
	emitBlendNormalMap_->frequencyTime += kDeltaTime_;

	// 射出間隔を上回ったら射出許可を出して時間を調整
	if (emitBlendNormalMap_->frequency <= emitBlendNormalMap_->frequencyTime) {
		emitBlendNormalMap_->frequencyTime -= emitBlendNormalMap_->frequency;
		emitBlendNormalMap_->emit = 1;
	}
	// 射出間隔を上回っていないので、射出許可は出ない
	else {
		emitBlendNormalMap_->emit = 0;
	}

	// 時間経過
	perFrameMap_->time_ += perFrameMap_->deltaTime_;

}

void ExplodeParticle::Draw(ID3D12GraphicsCommandList* commandList, BaseCamera& camera)
{

	assert(commandList);

	// GPUParticleViewのマッピング
	GPUParticleViewMapping(camera);

	// エミット
	Emit(commandList);

	// バリア
	UAVBarrier(commandList);

	// 更新
	UpdateCS(commandList);

	// リソースバリア
	ResouseBarrierToNonPixelShader(commandList);

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//形状を設定。PS0に設定しているものとは別。
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// パイプライン
	commandList->SetPipelineState(pipelineState_);
	// ルートシグネチャ
	commandList->SetGraphicsRootSignature(rootSignature_);

	// 頂点データ
	commandList->IASetVertexBuffers(0, 1, eggModel_->GetMesh()->GetVbView());

	// GPUパーティクル用
	commandList->SetGraphicsRootDescriptorTable(0, srvHandleGPU_);
	// GPUパーティクルのView
	commandList->SetGraphicsRootConstantBufferView(1, gpuParticleViewBuff_->GetGPUVirtualAddress());
	// テクスチャ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		commandList,
		2,
		eggModel_->GetTextureHandles()[0]);
	// マテリアル
	commandList->SetGraphicsRootConstantBufferView(3, material_->GetMaterialBuff()->GetGPUVirtualAddress());

	// 描画
	commandList->DrawInstanced(UINT(eggModel_->GetModelData().vertices.size()), GPUParticle::kParticleMax_, 0, 0);

	// リソースバリア
	ResouseBarrierToUnorderedAccess(commandList);

}

void ExplodeParticle::UAVBufferInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{


	// バッファ
	buff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(ParticleBlendNormalCS) + 0xff) & ~0xff) * GPUParticle::kParticleMax_);

	/// UAV

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = GPUParticle::kParticleMax_;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(ParticleBlendNormalCS);

	uavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	uavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	uavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(buff_.Get(), nullptr, &uavDesc, uavHandleCPU_);

	/// ここまでUAV

	/// SRV

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = kParticleMax_;
	srvDesc.Buffer.StructureByteStride = sizeof(ParticleBlendNormalCS);

	srvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	srvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	srvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(buff_.Get(), &srvDesc, srvHandleCPU_);

	/// ここまでSRV

	// フリーリストインデックスUAVバッファ
	freeListIndexBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(int32_t) + 0xff) & ~0xff));

	D3D12_UNORDERED_ACCESS_VIEW_DESC freeListIndexUavDesc{};

	freeListIndexUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	freeListIndexUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	freeListIndexUavDesc.Buffer.FirstElement = 0;
	freeListIndexUavDesc.Buffer.NumElements = 1;
	freeListIndexUavDesc.Buffer.CounterOffsetInBytes = 0;
	freeListIndexUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	freeListIndexUavDesc.Buffer.StructureByteStride = sizeof(int32_t);

	freeListIndexHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	freeListIndexHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	freeListIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(freeListIndexBuff_.Get(), nullptr, &freeListIndexUavDesc, freeListIndexHandleCPU_);


	// フリーリストUAVバッファ
	freeListBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(uint32_t) + 0xff) & ~0xff) * kParticleMax_);

	D3D12_UNORDERED_ACCESS_VIEW_DESC freeListUavDesc{};

	freeListUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	freeListUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	freeListUavDesc.Buffer.FirstElement = 0;
	freeListUavDesc.Buffer.NumElements = kParticleMax_;
	freeListUavDesc.Buffer.CounterOffsetInBytes = 0;
	freeListUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	freeListUavDesc.Buffer.StructureByteStride = sizeof(uint32_t);

	freeListHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	freeListHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	freeListDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(freeListBuff_.Get(), nullptr, &freeListUavDesc, freeListHandleCPU_);

	// CSによる初期化
	InitialzieCS(commandList);


}

void ExplodeParticle::ConstantBufferInitialzie(ID3D12Device* device)
{

	//GPUParticleViewを作る
	gpuParticleViewBuff_ = BufferResource::CreateBufferResource(device, (sizeof(GPUParticleView) + 0xff) & ~0xff);
	//書き込むためのアドレスを取得
	gpuParticleViewBuff_->Map(0, nullptr, reinterpret_cast<void**>(&gpuParticleViewMap_));

	// マッピング
	gpuParticleViewMap_->billboardMatrix = Matrix4x4::MakeIdentity4x4();
	gpuParticleViewMap_->viewProjection = Matrix4x4::MakeIdentity4x4();

	//エミッタを作る
	emitterBuff_ = BufferResource::CreateBufferResource(device, (sizeof(EmitBlendNormalCS) + 0xff) & ~0xff);
	//書き込むためのアドレスを取得
	emitterBuff_->Map(0, nullptr, reinterpret_cast<void**>(&emitBlendNormalMap_));

	// マッピング
	emitBlendNormalMap_->count = 10;
	emitBlendNormalMap_->frequency = 0.5f;
	emitBlendNormalMap_->frequencyTime = 0.0f;
	emitBlendNormalMap_->translate0 = Vector3(0.0f, 0.0f, 0.0f);
	emitBlendNormalMap_->translate1 = Vector3(0.0f, 0.0f, 0.0f);
	emitBlendNormalMap_->translate2 = Vector3(0.0f, 0.0f, 0.0f);
	emitBlendNormalMap_->translate3 = Vector3(0.0f, 0.0f, 0.0f);
	emitBlendNormalMap_->num = 0;
	emitBlendNormalMap_->radius = 1.0f;
	emitBlendNormalMap_->emit = 0;

	// 時間バッファ
	perFrameBuff_ = BufferResource::CreateBufferResource(device, (sizeof(PerFrame) + 0xff) & ~0xff);
	//書き込むためのアドレスを取得
	perFrameBuff_->Map(0, nullptr, reinterpret_cast<void**>(&perFrameMap_));

	// 時間マップ
	perFrameMap_->deltaTime_ = kDeltaTime_;
	perFrameMap_->time_ = 0.0f;

}

void ExplodeParticle::InitialzieCS(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexInitialize].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexInitialize].Get());

	commandList->SetComputeRootDescriptorTable(0, uavHandleGPU_);

	commandList->SetComputeRootDescriptorTable(1, freeListIndexHandleGPU_);

	commandList->SetComputeRootDescriptorTable(2, freeListHandleGPU_);

	commandList->Dispatch(1, 1, 1);

}

void ExplodeParticle::Emit(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexEmit].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexEmit].Get());

	commandList->SetComputeRootDescriptorTable(0, uavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, emitterBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, perFrameBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(3, freeListIndexHandleGPU_);

	commandList->SetComputeRootDescriptorTable(4, freeListHandleGPU_);

	commandList->Dispatch(1, 1, 1);

}

void ExplodeParticle::UpdateCS(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexUpdate].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexUpdate].Get());

	commandList->SetComputeRootDescriptorTable(0, uavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, perFrameBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(2, freeListIndexHandleGPU_);

	commandList->SetComputeRootDescriptorTable(3, freeListHandleGPU_);

	commandList->Dispatch(1, 1, 1);

}

void ExplodeParticle::SetEmitter(const EmitBlendNormalCS& emitter, bool isEmitSet)
{

	// マッピング
	emitBlendNormalMap_->count = emitter.count;
	emitBlendNormalMap_->frequency = emitter.frequency;
	emitBlendNormalMap_->translate0 = emitter.translate0;
	emitBlendNormalMap_->translate1 = emitter.translate1;
	emitBlendNormalMap_->translate2 = emitter.translate2;
	emitBlendNormalMap_->translate3 = emitter.translate3;
	emitBlendNormalMap_->num = emitter.num;
	emitBlendNormalMap_->radius = emitter.radius;

	emitBlendNormalMap_->pad0 = 0.0f;
	emitBlendNormalMap_->pad1 = 0.0f;
	emitBlendNormalMap_->pad2 = 0.0f;

	if (isEmitSet) {
		emitBlendNormalMap_->emit = emitter.emit;
		emitBlendNormalMap_->frequencyTime = emitter.frequencyTime;
	}

}

void ExplodeParticle::PipelineStateCSInitializeForInitialize(ID3D12Device* device)
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListIndexDescriptorRange[1] = {};
	freeListIndexDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	freeListIndexDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListIndexDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListIndexDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListDescriptorRange[1] = {};
	freeListDescriptorRange[0].BaseShaderRegister = 2;//iから始まる
	freeListDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = freeListIndexDescriptorRange;//Tableの中身の配列を指定
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(freeListIndexDescriptorRange);//Tableで利用する数

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = freeListDescriptorRange;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(freeListDescriptorRange);//Tableで利用する数

	descriptionRootsignature.pParameters = rootParameters; //ルートパラメータ配列へのポインタ
	descriptionRootsignature.NumParameters = _countof(rootParameters); //配列の長さ

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
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexInitialize]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/GPUParticle/ExplodeParticle/ExplodeInitialize.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexInitialize].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexInitialize]));
	assert(SUCCEEDED(hr));

}

void ExplodeParticle::PipelineStateCSInitializeForEmit(ID3D12Device* device)
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[5] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListIndexDescriptorRange[1] = {};
	freeListIndexDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	freeListIndexDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListIndexDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListIndexDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListDescriptorRange[1] = {};
	freeListDescriptorRange[0].BaseShaderRegister = 2;//iから始まる
	freeListDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[2].Descriptor.ShaderRegister = 1;//レジスタ番号indexとバインド

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = freeListIndexDescriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(freeListIndexDescriptorRange);//Tableで利用する数

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[4].DescriptorTable.pDescriptorRanges = freeListDescriptorRange;//Tableの中身の配列を指定
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(freeListDescriptorRange);//Tableで利用する数

	descriptionRootsignature.pParameters = rootParameters; //ルートパラメータ配列へのポインタ
	descriptionRootsignature.NumParameters = _countof(rootParameters); //配列の長さ

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
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexEmit]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/GPUParticle/ExplodeParticle/ExplodeEmit.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexEmit].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexEmit]));
	assert(SUCCEEDED(hr));

}

void ExplodeParticle::PipelineStateCSInitializeForUpdate(ID3D12Device* device)
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[4] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListIndexDescriptorRange[1] = {};
	freeListIndexDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	freeListIndexDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListIndexDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListIndexDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListDescriptorRange[1] = {};
	freeListDescriptorRange[0].BaseShaderRegister = 2;//iから始まる
	freeListDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = freeListIndexDescriptorRange;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(freeListIndexDescriptorRange);//Tableで利用する数

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = freeListDescriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(freeListDescriptorRange);//Tableで利用する数

	descriptionRootsignature.pParameters = rootParameters; //ルートパラメータ配列へのポインタ
	descriptionRootsignature.NumParameters = _countof(rootParameters); //配列の長さ

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
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexUpdate]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/GPUParticle/ExplodeParticle/ExplodeUpdate.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexUpdate].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexUpdate]));
	assert(SUCCEEDED(hr));

}
