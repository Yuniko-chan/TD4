#include "GameSceneObjectManager.h"
#include "../Factory/ObjectFactory.h"
#include "../Factory/CreateObjectNames.h"
#include "../Player/Player.h"

void GameSceneObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	// オブジェクトファクトリー
	objectFactory_ = std::make_unique<ObjectFactory>();
	static_cast<ObjectFactory*>(objectFactory_.get())->Initialize(this);

	levelDataManager_ = levelDataManager;
	BaseObjectManager::Initialize(levelIndex, levelDataManager_);

	objectsDeletionPosition_ = static_cast<uint32_t>(objects_.size());

	player_ = static_cast<Player*>(GetObjectPointer("Player"));

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

	// レベル
	level_ = 0;

	// オブジェクト読み込み数
	numberOfObjectsRead_ = 0;

	// オブジェクト読み込み中
	loadingObject_ = false;

}

void GameSceneObjectManager::Update()
{

	// オブジェクトマネージャー
	if (!loadingObject_) {
		BaseObjectManager::Update();
	}

	// レベル変更確認
	if (player_->GetWarping()) {
		LevelChange();
		player_->WarpPostProcessing();
	}

	// データ読み込み
	if (loadingObject_) {
		LevelIndex levelIndex = static_cast<LevelIndex>(kLevelIndexGenerationPattern_00 + level_);
		GeneratePattern(levelIndex, levelDataManager_);
	}

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

void GameSceneObjectManager::LevelChange()
{

	// 最大レベル
	const uint32_t kLevelMax = 2;

	// レベルアップ
	if (player_->GetLevelUp()) {
		level_++;
	}

	if (level_ >= kLevelMax) {
		levelChangeEnd_ = true;
		return;
	}

	// 読み込み開始
	loadingObject_ = true;

}

void GameSceneObjectManager::GeneratePattern(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	// レベルデータの取得
	LevelData* levelData = levelDataManager->GetLevelDatas(levelIndex);

	// 読み込み回数
	uint32_t countOfObjectsRead = 0;
	const uint32_t kCountOfObjectsReadMax = 1;

	// レベルデータのオブジェクトを走査
	std::vector<LevelData::ObjectData>::iterator it = levelData->objectsData_.begin();
	it += numberOfObjectsRead_;
	for (;it != levelData->objectsData_.end(); ++it) {

		// オブジェクトの参照
		LevelData::ObjectData objectData = *it;

		// 型にあわせてInitialize
		std::unique_ptr<IObject> object;
		object.reset(static_cast<ObjectFactory*>(objectFactory_.get())->CreateObjectPattern(objectData));

		if (object) {
			// listへ
			loadObjects_.emplace_back(object->GetName(), std::move(object));
		}
		numberOfObjectsRead_++;
		countOfObjectsRead++;

		if (kCountOfObjectsReadMax <= countOfObjectsRead) {
			break;
		}

	}

	// 読み込みが全て終わったら
	if (levelData->objectsData_.size() == loadObjects_.size()) {
		// オブジェクトを消して切り替える
		uint32_t count = 0;
		objects_.remove_if([&](ObjectPair& objects) {
			count++;
			if (count > objectsDeletionPosition_) {
				objects.second.reset();
				return true;
			}
			return false;
			});
		objects_.splice(objects_.end(),loadObjects_);
		loadObjects_.clear();

		// 読み込み終了
		loadingObject_ = false;
		numberOfObjectsRead_ = 0;

	}

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
		if (objectIndex == kCreateObjectIndexPlayer ||
			objectIndex == kCreateObjectIndexGhost) {
			shadowManager_->CastsShadowObjListRegister(static_cast<MeshObject*>(it->second.get()));
		}
		// 影が写るオブジェクト
		else if (objectIndex == kCreateObjectIndexGroundBlock) {
			// 大きさの加算分
			const Vector3 kAddSize = { -0.5f, 0.0f, -0.5f };
			shadowManager_->ShadowAppearsObjListRegister(static_cast<MeshObject*>(it->second.get()), kAddSize);
		}

	}

	// 更新
	shadowManager_->Update();

}
