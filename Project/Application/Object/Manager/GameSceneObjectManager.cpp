#include "GameSceneObjectManager.h"
#include "../Factory/ObjectFactory.h"
#include "../Factory/CreateObjectNames.h"
#include "../GameObjectsList.h"
#include "../Car/Preset/VehiclePaths.h"

#include "../../../Engine/2D/ImguiManager.h"

void GameSceneObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	// オブジェクトファクトリー
	objectFactory_ = std::make_unique<ObjectFactory>();
	static_cast<ObjectFactory*>(objectFactory_.get())->Initialize(this);

	levelDataManager_ = levelDataManager;
	//BaseObjectManager::Initialize(levelIndex, levelDataManager_);
	// コライダーのデバッグ描画
	colliderDebugDraw_ = std::make_unique<ColliderDebugDraw>();
	colliderDebugDraw_->Initialize();

	// 影
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	shadowModel_.reset(Model::Create("Resources/Model/shadow/", "shadow.obj", dxCommon));
	shadowManager_ = std::make_unique<ShadowManager>();
	shadowManager_->Initialize(shadowModel_.get());

	// ピックアップ
	pickupPointManager_ = std::make_unique<PickupPointManager>();
	pickupPointManager_->SetObjectManager(this);


	// 初期読み込み 
	// レベルデータの取得
	/*LevelData* levelData = levelDataManager->GetLevelDatas(kLevelIndexGenerationPattern_00);
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

	}*/

	LevelData* levelData = levelDataManager->GetLevelDatas(levelIndex);
	// レベルデータのオブジェクトを走査
	for (std::vector<LevelData::ObjectData>::iterator it = levelData->objectsData_.begin(); it != levelData->objectsData_.end(); ++it) {

		// オブジェクトの参照
		LevelData::ObjectData objectData = *it;

		// 型にあわせてInitialize
		std::unique_ptr<IObject> object;
		object.reset(static_cast<ObjectFactory*>(objectFactory_.get())->CreateObjectPattern(objectData));

		//PickupPoint登録
		if (std::holds_alternative<LevelData::MeshData>(objectData)) {

			LevelData::MeshData meshData = std::get<LevelData::MeshData>(objectData);

			RegisterPickupPoint(object.get(), meshData.className);
		}
		if (object) {
			// listへ
			objects_.emplace_back(object->GetName(), std::move(object));
		}

	}

	// インスタンシング描画
	instancingDrawing_ = std::make_unique<GameSceneInstancingDrawing>();
	instancingDrawing_->Initialize();

	// プレイヤーの設定などなど
	PlayerInitialize();

	// オプション関数（ハードコード
	OptionProcess();
}

void GameSceneObjectManager::Update()
{
	BaseObjectManager::Update();

	// 影
	ShadowUpdate();

}

void GameSceneObjectManager::Draw(BaseCamera& camera)
{

	instancingDrawing_->Clear();

	// オブジェクト走査
	for (std::list<ObjectPair>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {

		MeshObject* object = static_cast<MeshObject*>(it->second.get());

		// インスタンシング描画登録成功しないなら単独描画
		if (!instancingDrawing_->RegistrationConfirmation(
			object, camera.GetViewProjectionMatrix())) {

			static_cast<MeshObject*>(it->second.get())->Draw(camera);

		}

	}

	// インスタンシング描画
	instancingDrawing_->Draw(camera);

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

void GameSceneObjectManager::ImGuiDraw()
{
	// ベース分
	BaseObjectManager::ImGuiDraw();

	// 追加分
#ifdef _DEMO
	
	partsManager_->ImGuiDraw();

	ImGui::Begin("GameObjectManager");
	static char buffer[256];
	ImGui::InputText("PresetName", buffer, 256);

	if (ImGui::Button("プリセット生成")) {
		VehiclePreset(buffer);
	}
	static Vector3 spownPoint = Vector3(0, 0, 0);
	ImGui::DragFloat3("SpownPoint", &spownPoint.x, 0.01f);

	if (ImGui::Button("タイヤ追加")) {
		std::string name = "Tire" + std::to_string(Car::SerialNumberGenerate::sSerialTire);
		AddObject("TireParts", name.c_str(), "Resources/Model/Tire", "Tire.obj", spownPoint);
		partsManager_->AddParts(name, static_cast<Car::IParts*>(this->GetObjectPointer(name)));
		Car::SerialNumberGenerate::sSerialTire++;
	}

	if (ImGui::Button("フレーム追加")) {
		std::string name = "ArmorFrame" + std::to_string(Car::SerialNumberGenerate::sSerialArmor);
		AddObject("ArmorFrameParts", name.c_str(), "Resources/Model/Frame", "Frame.obj", spownPoint);
		partsManager_->AddParts(name, static_cast<Car::IParts*>(this->GetObjectPointer(name)));
		Car::SerialNumberGenerate::sSerialArmor++;
	}

	if (ImGui::Button("エンジン追加")) {
		std::string name = "Engine" + std::to_string(Car::SerialNumberGenerate::sSerialEngine);
		AddObject("EngineParts", name.c_str(), "Resources/Model/Engine", "Engine.obj", spownPoint);
		partsManager_->AddParts(name, static_cast<Car::IParts*>(this->GetObjectPointer(name)));
		Car::SerialNumberGenerate::sSerialEngine++;
	}

	ImGui::End();

#endif // _DEMO


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

void GameSceneObjectManager::AddObject(const std::string& className, const std::string& directory, const std::string& modelName)
{
	std::unique_ptr<IObject> object;
	LevelData::MeshData meshData = {};
	meshData.name = className;
	meshData.className = className;
	meshData.directoryPath = directory;
	meshData.flieName = modelName;

	meshData.collider = OBB{};

	meshData.transform = {};
	meshData.transform.scale = { 1.0f,1.0f,1.0f };
	LevelData::ObjectData objData = meshData;
	object.reset(static_cast<ObjectFactory*>(objectFactory_.get())->CreateObjectPattern(objData));
	objects_.emplace_back(object->GetName(), std::move(object));
}

void GameSceneObjectManager::AddObject(const std::string& className, const std::string& name, const std::string& directory, const std::string& modelName)
{
	std::unique_ptr<IObject> object;
	LevelData::MeshData meshData = {};
	meshData.name = name;
	meshData.className = className;
	meshData.directoryPath = directory;
	meshData.flieName = modelName;

	meshData.collider = OBB{};

	meshData.transform = {};
	meshData.transform.scale = { 1.0f,1.0f,1.0f };
	LevelData::ObjectData objData = meshData;
	object.reset(static_cast<ObjectFactory*>(objectFactory_.get())->CreateObjectPattern(objData));
	objects_.emplace_back(object->GetName(), std::move(object));
}

void GameSceneObjectManager::AddObject(const std::string& className, const std::string& name, const std::string& directory, const std::string& modelName, const Vector3& position)
{
	std::unique_ptr<IObject> object;
	LevelData::MeshData meshData = {};
	meshData.name = name;
	meshData.className = className;
	meshData.directoryPath = directory;
	meshData.flieName = modelName;

	meshData.collider = OBB{};

	meshData.transform = {};
	meshData.transform.translate = position;
	meshData.transform.scale = { 1.0f,1.0f,1.0f };
	LevelData::ObjectData objData = meshData;
	object.reset(static_cast<ObjectFactory*>(objectFactory_.get())->CreateObjectPattern(objData));
	objects_.emplace_back(object->GetName(), std::move(object));
}

void GameSceneObjectManager::PlayerInitialize()
{
	// インタラクトのオブジェクト
	AddObject("InteractionSpot", sVehiclePaths[VehicleDatas::kArmor].first, sVehiclePaths[VehicleDatas::kArmor].second);
	// パーツ
	partsManager_ = std::make_unique<VehiclePartsManager>();
	// ピックアップ
	//pickupPointManager_ = std::make_unique<PickupPointManager>();
	//pickupPointManager_->SetObjectManager(this);

	Player* player = static_cast<Player*>(this->GetObjectPointer("Player"));
	player->GetPickUpManager()->SetPartsManager(partsManager_.get());
	player->GetPickUpManager()->SetPickupPointManager(pickupPointManager_.get());
	player->GetPickUpManager()->SetInteractSpot(static_cast<InteractionSpot*>(this->GetObjectPointer("InteractionSpot")));
}

void GameSceneObjectManager::OptionProcess()
{
	//AddObject("TerrainObject", "Resources/Model/Ground", "Ground.obj");

	//AddObject("EnginePickupPoint", sVehiclePaths[VehicleDatas::kEngine].first, sVehiclePaths[VehicleDatas::kEngine].second);
	//AddObject("TirePickupPoint", sVehiclePaths[VehicleDatas::kTire].first, sVehiclePaths[VehicleDatas::kTire].second);
	//AddObject("ArmorPickupPoint", sVehiclePaths[VehicleDatas::kArmor].first, sVehiclePaths[VehicleDatas::kArmor].second);
	//
	//pickupPointManager_->AddPickupPoint("EnginePickupPoint", static_cast<IPickupPoint*>(this->GetObjectPointer("EnginePickupPoint")));
	//pickupPointManager_->AddPickupPoint("TirePickupPoint", static_cast<IPickupPoint*>(this->GetObjectPointer("TirePickupPoint")));
	//pickupPointManager_->AddPickupPoint("ArmorPickupPoint", static_cast<IPickupPoint*>(this->GetObjectPointer("ArmorPickupPoint")));
	
	VehiclePreset("init");

	// コア作成
	//AddObject("VehicleCore", "Resources/Model/Frame", "Frame.obj");
	
	// キャスト
	//VehicleCore* core = static_cast<VehicleCore*>(this->GetObjectPointer("VehicleCore"));

	// ペアレント＋トランスフォーム親子設定
	//core->SetPlayer(player);
	//player->SetPair(core);
	//player->GetPickUpManager()->SetPartsManager(partsManager_.get());
	//partsManager_->AddParts(core->GetName(), core);

	//VehiclePreset("Test");
}

void GameSceneObjectManager::RegisterPickupPoint(IObject* object, const std::string& className) {
	for (size_t i = 0; i < kPickupPointCount_;i++) {
		if (className == kRegisterPickupPointNames_[i]) {
			pickupPointManager_->AddPickupPoint(className, static_cast<IPickupPoint*>(object));
		}
	}
}

void GameSceneObjectManager::VehiclePreset(const std::string& presetName)
{
	std::string name = presetName + "Core";
	AddObject("VehicleCore", name.c_str(),
		sVehiclePaths[VehicleDatas::kCore].first, sVehiclePaths[VehicleDatas::kCore].second);
	VehicleCore* core = static_cast<VehicleCore*>(this->GetObjectPointer(name));
	Player* player = static_cast<Player*>(this->GetObjectPointer("Player"));
	// ペアレント＋トランスフォーム親子設定
	core->SetPlayer(player);
	player->SetPair(core);
	partsManager_->AddParts(core->GetName(), core);


	name = presetName + "Engine";
	AddObject("EngineParts", name.c_str(),
		sVehiclePaths[VehicleDatas::kEngine].first, sVehiclePaths[VehicleDatas::kEngine].second);
	partsManager_->AddParts(name, static_cast<Car::IParts*>(this->GetObjectPointer(name)));
	core->GetConstructionSystem()->AnyDocking(static_cast<Car::IParts*>(this->GetObjectPointer(name)), Vector2Int(0, -1));
	static_cast<Car::IParts*>(this->GetObjectPointer(name))->SetParent(core);


	name = presetName + "Tire" + std::to_string(Car::SerialNumberGenerate::sSerialTire);
	Car::SerialNumberGenerate::sSerialTire++;
	AddObject("TireParts", name.c_str(),
		sVehiclePaths[VehicleDatas::kTire].first, sVehiclePaths[VehicleDatas::kTire].second);
	partsManager_->AddParts(name, static_cast<Car::IParts*>(this->GetObjectPointer(name)));
	core->GetConstructionSystem()->AnyDocking(static_cast<Car::IParts*>(this->GetObjectPointer(name)), Vector2Int(1, 0));
	static_cast<Car::IParts*>(this->GetObjectPointer(name))->SetParent(core);

	name = presetName + "Tire" + std::to_string(Car::SerialNumberGenerate::sSerialTire);
	Car::SerialNumberGenerate::sSerialTire++;
	AddObject("TireParts", name.c_str(),
		sVehiclePaths[VehicleDatas::kTire].first, sVehiclePaths[VehicleDatas::kTire].second);
	partsManager_->AddParts(name, static_cast<Car::IParts*>(this->GetObjectPointer(name)));
	core->GetConstructionSystem()->AnyDocking(static_cast<Car::IParts*>(this->GetObjectPointer(name)), Vector2Int(-1, 0));
	static_cast<Car::IParts*>(this->GetObjectPointer(name))->SetParent(core);


	name = presetName + "Armor";
	AddObject("ArmorFrameParts", name.c_str(),
		sVehiclePaths[VehicleDatas::kArmor].first, sVehiclePaths[VehicleDatas::kArmor].second);
	partsManager_->AddParts(name, static_cast<Car::IParts*>(this->GetObjectPointer(name)));
	core->GetConstructionSystem()->AnyDocking(static_cast<Car::IParts*>(this->GetObjectPointer(name)), Vector2Int(0, -2));
	static_cast<Car::IParts*>(this->GetObjectPointer(name))->SetParent(core);

}

void GameSceneObjectManager::AddObject(IObject* object) {
	std::unique_ptr<IObject> uniqueObject;
	uniqueObject.reset(object);
	objects_.emplace_back(uniqueObject->GetName(), std::move(uniqueObject));
}