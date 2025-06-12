#include "InstancingDrawing.h"
#include "ModelManager.h"
#include "../../base/BufferResource.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"

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
		instancingDrawingDatas_[i].buff = BufferResource::CreateBufferResource(device, ((sizeof(TransformationMatrix) + 0xff) & ~0xff) * kTransformationMatrixMax_);
		//書き込むためのアドレスを取得
		instancingDrawingDatas_[i].buff->Map(0, nullptr, reinterpret_cast<void**>(&instancingDrawingDatas_[i].map));

		D3D12_SHADER_RESOURCE_VIEW_DESC externalSrvDesc{};
		externalSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
		externalSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		externalSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		externalSrvDesc.Buffer.FirstElement = 0;
		externalSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		externalSrvDesc.Buffer.NumElements = kTransformationMatrixMax_;
		externalSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);

		instancingDrawingDatas_[i].srvHandleCPU = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
		instancingDrawingDatas_[i].srvHandleGPU = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
		instancingDrawingDatas_[i].srvIndexDescriptorHeap = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
		SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

		device->CreateShaderResourceView(instancingDrawingDatas_[i].buff.Get(), &externalSrvDesc, instancingDrawingDatas_[i].srvHandleCPU);

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

bool InstancingDrawing::RegistrationConfirmation(Model* model, WorldTransform* worldTransform, const Matrix4x4& viewProjectionMatrix)
{

	for (size_t i = 0; i < kModelDataMax_; ++i) {
		// 名前が一致する
		if (model->GetFileName() == instancingDrawingDatas_[i].model->GetFileName()) {
			
			// マップ
			instancingDrawingDatas_[i].map[instancingDrawingTransformationMatrixNum_[i]].WVP = worldTransform->worldMatrix_ * viewProjectionMatrix;
			instancingDrawingDatas_[i].map[instancingDrawingTransformationMatrixNum_[i]].World = worldTransform->worldMatrix_;
			instancingDrawingDatas_[i].map[instancingDrawingTransformationMatrixNum_[i]].WorldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldTransform->worldMatrix_));

			// 回数をインクリメント
			instancingDrawingTransformationMatrixNum_[i]++;
			// 登録成功
			return true;

		}
	
	}

	// 登録失敗
	return false;

}

void InstancingDrawing::Draw()
{
}
