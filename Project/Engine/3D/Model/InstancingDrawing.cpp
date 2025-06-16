#include "InstancingDrawing.h"
#include "ModelManager.h"
#include "../../base/BufferResource.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "ModelDraw.h"

void InstancingDrawing::Initialize()
{

	// モデルマネージャー
	ModelManager* modelManager = ModelManager::GetInstance();
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// 変数初期化
	for (size_t i = 0; i < kModelDataMax_; ++i) {
		
		// インスタンシング描画用モデル保存
		instancingDrawingDatas_[i].model = modelManager->GetModel(kModelDatas_[i].first.first, kModelDatas_[i].first.second);
		instancingDrawingDatas_[i].isAnimation = kModelDatas_[i].second;

		// バッファ
		instancingDrawingDatas_[i].transformBuff = BufferResource::CreateBufferResource(device, ((sizeof(TransformationMatrix) + 0xff) & ~0xff) * kTransformationMatrixMax_);
		//書き込むためのアドレスを取得
		instancingDrawingDatas_[i].transformBuff->Map(0, nullptr, reinterpret_cast<void**>(&instancingDrawingDatas_[i].transformMap));

		D3D12_SHADER_RESOURCE_VIEW_DESC transformSrvDesc{};
		transformSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
		transformSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		transformSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		transformSrvDesc.Buffer.FirstElement = 0;
		transformSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		transformSrvDesc.Buffer.NumElements = kTransformationMatrixMax_;
		transformSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);

		instancingDrawingDatas_[i].transformSrvHandleCPU = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
		instancingDrawingDatas_[i].transformSrvHandleGPU = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
		instancingDrawingDatas_[i].transformSrvIndexDescriptorHeap = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
		SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

		device->CreateShaderResourceView(instancingDrawingDatas_[i].transformBuff.Get(), &transformSrvDesc, instancingDrawingDatas_[i].transformSrvHandleCPU);

		// バッファ
		instancingDrawingDatas_[i].materialBuff = BufferResource::CreateBufferResource(device, ((sizeof(MaterialData) + 0xff) & ~0xff) * kTransformationMatrixMax_);
		//書き込むためのアドレスを取得
		instancingDrawingDatas_[i].materialBuff->Map(0, nullptr, reinterpret_cast<void**>(&instancingDrawingDatas_[i].materialMap));

		D3D12_SHADER_RESOURCE_VIEW_DESC materialSrvDesc{};
		materialSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
		materialSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		materialSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		materialSrvDesc.Buffer.FirstElement = 0;
		materialSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		materialSrvDesc.Buffer.NumElements = kTransformationMatrixMax_;
		materialSrvDesc.Buffer.StructureByteStride = sizeof(MaterialData);

		instancingDrawingDatas_[i].materialSrvHandleCPU = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
		instancingDrawingDatas_[i].materialSrvHandleGPU = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
		instancingDrawingDatas_[i].materialSrvIndexDescriptorHeap = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
		SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

		device->CreateShaderResourceView(instancingDrawingDatas_[i].materialBuff.Get(), &materialSrvDesc, instancingDrawingDatas_[i].materialSrvHandleCPU);

		// ワールドトランスフォームの保存回数
		instancingDrawingTransformationMatrixNum_[i] = 0;

	}

}

void InstancingDrawing::Clear()
{

	for (size_t i = 0; i < kModelDataMax_; ++i) {
		// ワールドトランスフォームの保存回数を0
		instancingDrawingTransformationMatrixNum_[i] = 0;
	}

}

bool InstancingDrawing::RegistrationConfirmation(
	Model* model, 
	WorldTransform* worldTransform,
	const MaterialData& materialData, 
	const Matrix4x4& viewProjectionMatrix)
{

	for (size_t i = 0; i < kModelDataMax_; ++i) {
		// 名前が一致する
		if (model->GetFileName() == instancingDrawingDatas_[i].model->GetFileName()) {
			
			// トランスフォーム マップ
			instancingDrawingDatas_[i].transformMap[instancingDrawingTransformationMatrixNum_[i]].WVP = worldTransform->worldMatrix_ * viewProjectionMatrix;
			instancingDrawingDatas_[i].transformMap[instancingDrawingTransformationMatrixNum_[i]].World = worldTransform->worldMatrix_;
			instancingDrawingDatas_[i].transformMap[instancingDrawingTransformationMatrixNum_[i]].WorldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldTransform->worldMatrix_));

			// マテリアルデータ マップ
			instancingDrawingDatas_[i].materialMap[instancingDrawingTransformationMatrixNum_[i]].color = materialData.color;
			instancingDrawingDatas_[i].materialMap[instancingDrawingTransformationMatrixNum_[i]].enableLighting = materialData.enableLighting;
			instancingDrawingDatas_[i].materialMap[instancingDrawingTransformationMatrixNum_[i]].environmentCoefficient = materialData.environmentCoefficient;
			instancingDrawingDatas_[i].materialMap[instancingDrawingTransformationMatrixNum_[i]].shininess = materialData.shininess;
			instancingDrawingDatas_[i].materialMap[instancingDrawingTransformationMatrixNum_[i]].uvTransform = materialData.uvTransform;

			// 回数をインクリメント
			instancingDrawingTransformationMatrixNum_[i]++;

			// 登録成功
			return true;

		}
	
	}

	// 登録失敗
	return false;

}

void InstancingDrawing::Draw(BaseCamera& camera)
{

	for (size_t i = 0; i < kModelDataMax_; ++i) {
		// アニメーションあり
		if (instancingDrawingDatas_[i].isAnimation) {

			// 作成中
			

		}
		// アニメーションなし
		else {

			ModelDraw::ManyNormalObjectsDesc desc;
			desc.camera = &camera;
			desc.materialsHandle = &instancingDrawingDatas_[i].materialSrvHandleGPU;
			desc.model = instancingDrawingDatas_[i].model;
			desc.numInstance = static_cast<uint32_t>(instancingDrawingTransformationMatrixNum_[i]);
			desc.transformationMatrixesHandle = &instancingDrawingDatas_[i].transformSrvHandleGPU;
			ModelDraw::ManyNormalObjectsDraw(desc);
		
		}
	
	}

}
