#include "Model.h"
#include "../../base/Texture/TextureManager.h"

/// <summary>
/// 3Dモデル生成
/// </summary>
/// <returns></returns>
Model* Model::CreateFromModelData(Model::ModelData& modelData, const std::string& filename) {

	// 3Dオブジェクトのインスタンスを生成
	Model* object3d = new Model();
	assert(object3d);

	// 初期化
	object3d->InitializeFromModelData(modelData,filename);

	return object3d;

}

/// <summary>
/// 初期化
/// </summary>
void Model::InitializeFromModelData(Model::ModelData& modelData,const std::string& filename) {

	assert(sDevice_);

	// モデル読み込み
	modelData_ = modelData;

	// ファイルの名前
	fileName_ = filename;

	//メッシュ生成
	mesh_ = std::make_unique<Mesh>();
	mesh_->CreateMesh(sDevice_, modelData_.vertices, modelData_.vertexInfluences, modelData_.indices,DirectXCommon::GetInstance()->GetCommadListLoad());

	for (size_t i = 0; i < modelData_.material.textureFilePaths.size(); ++i) {
		// テクスチャハンドル
		textureHandles_.push_back(TextureManager::Load(modelData_.material.textureFilePaths[i], DirectXCommon::GetInstance()));
		// リソース
		resourceDescs_.push_back(TextureManager::GetInstance()->GetResourceDesc(textureHandles_[i]));
	}

}