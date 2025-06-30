#include "BaseInstancingDrawing.h"
#include "ModelManager.h"
#include "../../base/BufferResource.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "ModelDraw.h"

void BaseInstancingDrawing::Initialize()
{

	// モデルのデータがない
	if (modelDataMax_ == 0) {
		return;
	}

	// モデルマネージャー
	ModelManager* modelManager = ModelManager::GetInstance();
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// 変数初期化
	for (size_t i = 0; i < modelDataMax_; ++i) {
		
		// インスタンシング描画用モデル保存
		instancingDrawingDatas_[i].model = modelManager->GetModel(modelDatas_[i].first.first, modelDatas_[i].first.second);
		instancingDrawingDatas_[i].isAnimation = modelDatas_[i].second;
		
		// アニメーション
		if (instancingDrawingDatas_[i].isAnimation) {
			
			std::unique_ptr<BaseInstancingDrawingAnimation> animation;
			animation = std::make_unique<BaseInstancingDrawingAnimation>();
			animation->Initialize(instancingDrawingDatas_[i].model);
			animations_.push_back(std::move(animation));

			instancingDrawingDatas_[i].localMatrixManager = animations_[animations_.size() - 1].get()->GetLocalMatrixManager();
		}
		else {
			instancingDrawingDatas_[i].localMatrixManager = nullptr;
		}

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
		instancingDrawingDatas_[i].materialBuff = BufferResource::CreateBufferResource(device, ((sizeof(SRVMaterialData) + 0xff) & ~0xff) * kTransformationMatrixMax_);
		//書き込むためのアドレスを取得
		instancingDrawingDatas_[i].materialBuff->Map(0, nullptr, reinterpret_cast<void**>(&instancingDrawingDatas_[i].materialMap));

		D3D12_SHADER_RESOURCE_VIEW_DESC materialSrvDesc{};
		materialSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
		materialSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		materialSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		materialSrvDesc.Buffer.FirstElement = 0;
		materialSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		materialSrvDesc.Buffer.NumElements = kTransformationMatrixMax_;
		materialSrvDesc.Buffer.StructureByteStride = sizeof(SRVMaterialData);

		instancingDrawingDatas_[i].materialSrvHandleCPU = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
		instancingDrawingDatas_[i].materialSrvHandleGPU = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
		instancingDrawingDatas_[i].materialSrvIndexDescriptorHeap = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
		SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

		device->CreateShaderResourceView(instancingDrawingDatas_[i].materialBuff.Get(), &materialSrvDesc, instancingDrawingDatas_[i].materialSrvHandleCPU);

		// ワールドトランスフォームの保存回数
		instancingDrawingTransformationMatrixNum_[i] = 0;

	}

}

void BaseInstancingDrawing::Update()
{

	// アニメーション 0番のみ
	for (uint32_t i = 0; i < animations_.size(); ++i) {
		animations_[i]->Update(0);
	}

}

void BaseInstancingDrawing::Clear()
{

	for (size_t i = 0; i < modelDataMax_; ++i) {
		// ワールドトランスフォームの保存回数を0
		instancingDrawingTransformationMatrixNum_[i] = 0;
	}

}

bool BaseInstancingDrawing::RegistrationConfirmation(
	MeshObject* meshObject,
	const Matrix4x4& viewProjectionMatrix)
{

	Model* model = meshObject->GetModel();
	WorldTransform* worldTransform = meshObject->GetWorldTransformAdress();
	const MaterialData materialData = *(meshObject->GetMaterial()->GetMaterialMap());

	for (size_t i = 0; i < modelDataMax_; ++i) {
		// 名前が一致する
		if (model->GetFileName() == instancingDrawingDatas_[i].model->GetFileName()) {
			
			assert(!(kTransformationMatrixMax_ == instancingDrawingTransformationMatrixNum_[i]));

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

void BaseInstancingDrawing::Draw(BaseCamera& camera)
{

	for (size_t i = 0; i < modelDataMax_; ++i) {
		// アニメーションあり
		if (instancingDrawingDatas_[i].isAnimation) {

			// 作成中
			if (instancingDrawingDatas_[i].localMatrixManager) {
				ModelDraw::ManyAnimObjectsDesc desc;
				desc.camera = &camera;
				desc.materialsHandle = &instancingDrawingDatas_[i].materialSrvHandleGPU;
				desc.model = instancingDrawingDatas_[i].model;
				desc.numInstance = static_cast<uint32_t>(instancingDrawingTransformationMatrixNum_[i]);
				desc.transformationMatrixesHandle = &instancingDrawingDatas_[i].transformSrvHandleGPU;
				desc.localMatrixManager = instancingDrawingDatas_[i].localMatrixManager;
				ModelDraw::ManyAnimObjectsDraw(desc);
			}
			// ローカル行列マネージャーが入っていないならアニメーションなし
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

void BaseInstancingDrawing::SetLocalMatrixManager(const std::string& fileName, LocalMatrixManager* localMatrixManager)
{

	for (size_t i = 0; i < modelDataMax_; ++i) {
		// 名前が一致する
		if (fileName == instancingDrawingDatas_[i].model->GetFileName()) {
			instancingDrawingDatas_[i].localMatrixManager = localMatrixManager;
			break;
		}
	}

}
