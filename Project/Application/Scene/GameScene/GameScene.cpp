#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/Debug/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/base/WindowSprite/WindowSprite.h"
#include "../../Object/Manager/GameSceneObjectManager.h"
#include "../../Object/Factory/ObjectFactory.h"
#include "../../Object/Player/Player.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"
#include "../../Object/Car/VehicleCore.h"
#include "../../Object/CustomArea/CustomArea.h"
#include "../../Object/Gimmick/Cannon/Cannon.h"
#include "../../Object/Gimmick/Minigun/Minigun.h"

GameScene::~GameScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	BaseScene::Initialize();

	ModelCreate();
	TextureLoad();

	// パラメータマネージャ
	parameterManager_ = GlobalParameterManager::GetInstance();
	parameterManager_->Initialize();

	// ビュープロジェクション
	const EulerTransform baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.58f,0.0f,0.0f,
		0.0f, 23.0f, -35.0f };
	camera_.SetTransform(baseCameraTransform);

	// エフェクトマネージャー
	effectManager_ = EffectManager::GetInstance();
	effectManager_->Initialize(particleCircleModel_.get());

	isDebugCameraActive_ = false;

	collisionManager_.reset(new GameSceneCollisionManager);
	collisionManager_->Initialize();

	// 布
	ClothManager::GetInstance()->SetLight(
		directionalLight_.get(),
		pointLightManager_.get(),
		spotLightManager_.get());

	// オブジェクトマネージャー
	objectManager_ = std::make_unique<GameSceneObjectManager>();
	objectManager_->Initialize(kLevelIndexDebug, levelDataManager_);

	// キーコンフィグ
	keyConfig_ = GameKeyconfig::GetInstance();
	keyConfig_->Initialize();

	// タイマー
	gameTimeSystem_ = std::make_unique<GameTimeSystem>();
	gameTimeSystem_->Initialize();

	// カメラのマネージャー
	cameraManager_ = std::make_unique<GameCameraManager>();
	cameraManager_->Initialize();
	static_cast<FollowCamera*>(cameraManager_->FindCamera("Follow"))->SetTarget(objectManager_->GetObjectPointer("Player")->GetWorldTransformAdress());
	static_cast<OverheadCamera*>(cameraManager_->FindCamera("Overhead"))->SetTarget(objectManager_->GetObjectPointer("Player")->GetWorldTransformAdress());
	static_cast<Player*>(objectManager_->GetObjectPointer("Player"))->SetCameraManager(cameraManager_.get());
	//// 追従カメラ
	//followCamera_ = std::make_unique<FollowCamera>();
	//followCamera_->Initialize();
	//followCamera_->SetTarget(objectManager_->GetObjectPointer("Player")->GetWorldTransformAdress());
	//static_cast<Player*>(objectManager_->GetObjectPointer("Player"))->SetCamera(followCamera_.get());
	// UIマネージャー
	uiManager_ = std::make_unique<UIManager>();
	uiManager_->Initialize();

	// ポストエフェクト
	postEffectSystem_ = std::make_unique<PostEffectSystem>();
	postEffectSystem_->Initialize();
	postEffectSystem_->SetRenderTargetTexture(renderTargetTexture_);
	postEffectSystem_->SetDriveSystem(
		reinterpret_cast<VehicleCore*>(objectManager_->GetObjectPointer("initCore"))->GetDriveSystem());
	postEffectSystem_->SetPlayer(reinterpret_cast<Player*>(objectManager_->GetObjectPointer("Player")));

	// コース
	CourseInitialize();

	// モデル描画
	ModelDraw::PreDrawParameters preDrawParameters;
	preDrawParameters.directionalLight = directionalLight_.get();
	preDrawParameters.fogManager = FogManager::GetInstance();
	preDrawParameters.pointLightManager = pointLightManager_.get();
	preDrawParameters.spotLightManager = spotLightManager_.get();
	preDrawParameters.environmentTextureHandle = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());
	ModelDraw::SetPreDrawParameters(preDrawParameters);

	BaseScene::InitilaizeCheck();

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {

#ifdef _DEMO
	ImguiDraw();
#endif

//コースデバッグ用
#ifdef _DEMO
	if (input_->TriggerKey(DIK_P)) {
		AddCourse();
	}
#endif

	if (requestSceneNo_ == kClear || requestSceneNo_ == kTitle || isBeingReset_) {
		resetScene_ = false;
		return;
	}

	// 中間プレイ会用処理（初期化）
	if (input_->TriggerKey(DIK_0)) {
		Initialize();
	}
	else if (input_->TriggerJoystick(JoystickButton::kJoystickButtonSTART)) {
		Initialize();
	}

	// パラメータ
	parameterManager_->Update();
	gameTimeSystem_->Update();


	courseManager_->Update();

	//光源
	directionalLight_->Update(directionalLightData_);
	pointLightManager_->Update(pointLightDatas_);
	spotLightManager_->Update(spotLightDatas_);

	// カメラ
	//followCamera_->Update();
	//camera_ = static_cast<BaseCamera>(*followCamera_.get());

	cameraManager_->Update();
	camera_ = *cameraManager_->GetActiveCamera();

	// キー入力更新
	keyConfig_->Update();

	// オブジェクトマネージャー
	objectManager_->Update();

	// あたり判定
	collisionManager_->ListClear();

	objectManager_->CollisionListRegister(collisionManager_.get());

	collisionManager_->CheakAllCollision();

	courseCollisionSystem_->ObjectRegistration(objectManager_.get());
	courseCollisionSystem_->Execute();

	// コースデバッグ描画
	courseDebugDraw_->DrawMap(drawLine_);

	// デバッグカメラ
	DebugCameraUpdate();
	// エフェクトマネージャー
	effectManager_->Update(camera_);
	// UIマネージャー
	uiManager_->Update();
	// ポストエフェクト
	postEffectSystem_->Update();

	// ゲームが終了するか

}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

	//ゲームの処理 

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	renderTargetTexture_->ClearDepthBuffer();


#pragma endregion

#pragma region モデル描画

	ModelDraw::PreDraw(dxCommon_->GetCommadList());

	//3Dオブジェクトはここ

	objectManager_->Draw(camera_, drawLine_);

	// エフェクトマネージャー
	effectManager_->Draw(camera_);

	ModelDraw::PostDraw();

#pragma endregion

#pragma region 線描画

	drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

#pragma endregion

#pragma region パーティクル描画

	// パーティクル描画
	objectManager_->ParticleDraw(camera_);

#pragma endregion

	// ポストエフェクト
	postEffectSystem_->Execute();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());
	

	//背景
	//前景スプライト描画

	// UI
	uiManager_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::ImguiDraw(){

	ImGui::Begin("Framerate");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::Checkbox("DebugCamera", &isDebugCameraActive_);

	gameTimeSystem_->ImGuiDraw();
	ImGui::End();

	debugCamera_->ImGuiDraw();

	//followCamera_->ImGuiDraw();
	cameraManager_->ImGuiDraw();
	objectManager_->ImGuiDraw();

	PostEffect::GetInstance()->ImGuiDraw();

	uiManager_->ImGuiDraw();

	courseDebugDraw_->ImGuiDraw();

}

void GameScene::DebugCameraUpdate()
{

#ifdef _DEMO
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}
#endif

}

void GameScene::ModelCreate()
{

	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_));

}

void GameScene::TextureLoad()
{

}

void GameScene::CourseInitialize()
{

	// コース衝突システム
	courseCollisionSystem_ = std::make_unique<CourseCollisionSystem>();
	courseCollisionSystem_->Initialize();

	//コース生成システム
	courseManager_ = std::make_unique<CourseManager>();
	courseManager_->Initialize(static_cast<GameSceneObjectManager*>(objectManager_.get()),levelDataManager_);
	courseManager_->SetAddCourseFunction(std::bind(&GameScene::AddCourse,this));
	courseManager_->SetPlayer(reinterpret_cast<MeshObject*>(objectManager_->GetObjectPointer("Player")));


	// コースデバッグ描画
	courseDebugDraw_ = std::make_unique<CourseDebugDraw>();
	courseDebugDraw_->Initialize();

	// コース
	size_t courseGroupNum = 1;
	for (size_t i = 0; i < courseGroupNum; i++) {
		auto& courseList = courseManager_->GetCourseList(i);
		for (auto* course : courseList) {
			courseCollisionSystem_->SetCourse(course);
			courseDebugDraw_->SetCourse(course);
		}
	}

	// カスタムエリア
	CustomArea* customArea = nullptr;
	/*std::string customAreaName = "customArea";
	customArea = static_cast<CustomArea*>(objectManager_->GetObjectPointer(customAreaName));
	courseCollisionSystem_->SetCustomArea(customArea);
	customAreaName = "customArea.001";
	customArea = static_cast<CustomArea*>(objectManager_->GetObjectPointer(customAreaName));
	courseCollisionSystem_->SetCustomArea(customArea);*/
	for (size_t i = 0; i < courseGroupNum; i++) {
		std::string customAreaName = std::format("CustomArea{}",i);
		customArea = static_cast<CustomArea*>(objectManager_->GetObjectPointer(customAreaName));
		courseCollisionSystem_->SetCustomArea(customArea);
	}


	// 大砲
	Cannon* cannon = nullptr;
	size_t i = 0;
	while (1) {
		cannon = nullptr;
		std::string courseName = std::format("Cannon{}", i);
		cannon = static_cast<Cannon*>(objectManager_->GetObjectPointer(courseName));
		if (cannon) {
			courseCollisionSystem_->SetGimmick(reinterpret_cast<OBB*>(cannon->GetCollider()));
		}
		else {
			break;
		}
		++i;
	}

	// ミニガン
	Minigun* minigun = nullptr;
	i = 0;
	while (1) {
		minigun = nullptr;
		std::string courseName = std::format("Minigun{}", i);
		minigun = static_cast<Minigun*>(objectManager_->GetObjectPointer(courseName));
		if (minigun) {
			courseCollisionSystem_->SetGimmick(reinterpret_cast<OBB*>(minigun->GetCollider()));
		}
		else {
			break;
		}
		++i;
	}

}

void GameScene::AddCourse() {
	size_t group = courseManager_->AddCourseGroup();
	auto& courseList = courseManager_->GetCourseList(group);
	for (auto* course : courseList) {
		courseCollisionSystem_->SetCourse(course);
		courseDebugDraw_->SetCourse(course);
	}
	

	// カスタムエリア
	CustomArea* customArea = nullptr;
	std::string customAreaName = std::format("CustomArea{}", group);
	customArea = static_cast<CustomArea*>(objectManager_->GetObjectPointer(customAreaName));
	courseCollisionSystem_->SetCustomArea(customArea);
	
}