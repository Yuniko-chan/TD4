#include "GameSceneObjectManager.h"
#include "../Factory/ObjectFactory.h"
#include "../Factory/CreateObjectNames.h"

void GameSceneObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	// オブジェクトファクトリー
	objectFactory_ = std::make_unique<ObjectFactory>();
	static_cast<ObjectFactory*>(objectFactory_.get())->Initialize(this);

	levelDataManager_ = levelDataManager;
	BaseObjectManager::Initialize(levelIndex, levelDataManager_);

	// 影
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	shadowModel_.reset(Model::Create("Resources/Model/shadow/", "shadow.obj", dxCommon));
	shadowManager_ = std::make_unique<ShadowManager>();
	shadowManager_->Initialize(shadowModel_.get());

	// 初期読み込み 
	// レベルデータの取得
	LevelData* levelData = levelDataManager->GetLevelDatas(kLevelIndexGenerationPattern_00);
	// レベルデータのオブジェクトを走査
	for (std::vector<LevelData::ObjectData>::iterator it = levelData->objectsData_.begin(); it != levelData->objectsData_.end(); ++it) {

		// オブジェクトの参照
		LevelData::ObjectData objectData = *it;

		// 型にあわせてInitialize
		std::unique_ptr<IObject> object;
		object.reset(static_cast<ObjectFactory*>(objectFactory_.get())->CreateObjectPattern(objectData));

		if (object) {
			// listへ
			objects_.emplace_back(object->GetName(), std::move(object));
		}

	}

	std::unique_ptr<IObject> objectCarCore;
	LevelData::MeshData meshData = {};
	meshData.name = "CarCore";
	meshData.className = "CarCore";
	meshData.directoryPath = "Resources/Model/GroundBlock";
	meshData.flieName = "GroundBlock.obj";
	meshData.transform = {};
	meshData.transform.scale = { 1.0f,1.0f,1.0f };
	LevelData::ObjectData objData = meshData;
	objectCarCore.reset(static_cast<ObjectFactory*>(objectFactory_.get())->CreateObjectPattern(objData));
	objects_.emplace_back(objectCarCore->GetName(), std::move(objectCarCore));

}

void GameSceneObjectManager::Update()
{
	BaseObjectManager::Update();

	// 影
	ShadowUpdate();

}

void GameSceneObjectManager::Draw(BaseCamera& camera)
{

	// オブジェクトマネージャー
	BaseObjectManager::Draw(camera);

	// 影
	shadowManager_->Draw(camera);

}

void GameSceneObjectManager::Draw(BaseCamera& camera, DrawLine* drawLine)
{

	// オブジェクトマネージャー
	BaseObjectManager::Draw(camera, drawLine);

	// 影
	shadowManager_->Draw(camera);

}

void GameSceneObjectManager::ShadowUpdate()
{

	// 前処理
	shadowManager_->PreUpdate();

	// 更新処理
	for (std::list<ObjectPair>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {

		// オブジェクト名前(クラス)を取ってくる
		uint32_t objectIndex = 0;
		for (uint32_t i = 0; i < kCreateObjectIndexOfCount; i++) {
			if (it->second->GetClassNameString() == kCreateObjectNames_[i]) {
				objectIndex = i;
				break;
			}
		}

		// 影を出現させるオブジェクト
		//shadowManager_->CastsShadowObjListRegister(static_cast<MeshObject*>(it->second.get()));

		// 影が写るオブジェクト
		// 大きさの加算分
		//const Vector3 kAddSize = { -0.5f, 0.0f, -0.5f };
		//shadowManager_->ShadowAppearsObjListRegister(static_cast<MeshObject*>(it->second.get()), kAddSize);

	}

	// 更新
	shadowManager_->Update();

}
