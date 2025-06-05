#include "DebugScene.h"
#include "../../Course/CourseLoader.h"
#include "../../Object/Manager/GameSceneObjectManager.h"
DebugScene::~DebugScene()
{
}

void DebugScene::Initialize()
{
	BaseScene::Initialize();

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_));
	skydome_ = std::make_unique<Skydome>();
	LevelData::MeshData skydomeData;
	skydomeData.directoryPath = "Resources/Model/Skydome";
	skydomeData.flieName = "skydome.obj";
	skydomeData.transform = {1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	skydomeData.className = "skydome";
	skydomeData.name = "";
	skydomeData.parentName = "";
	skydome_->Initialize(&skydomeData);

	//clothDemo_ = std::make_unique<ClothDemo>();
	//clothDemo_->Initilalize(directionalLight_.get(), pointLightManager_.get(), spotLightManager_.get());

	//パーティクル
	ParticleManager_ = std::make_unique<ParticleManager>();
	ParticleManager_->Initialize();

	ParticleManager_->CreateParticle<RunDustParticle>("RunDust",
		{
				Vector3{0.0f, 0.0f, 0.0f}, // 位置
				1.0f, // 射出半径
				10, // 射出数
				0.1f, // 射出間隔
				0.0f, // 射出間隔調整時間
				true // 射出許可
		}
	);
	
	ParticleManager_->CreateParticle<GPUParticle>("");
	//UI
	UIManager_ = std::make_unique<UIManager>();
	UIManager_->Initialize();
	// オブジェクトマネージャー
	objectManager_ = std::make_unique<GameSceneObjectManager>();
	objectManager_->Initialize(kLevelIndexDebug, levelDataManager_);

	// コース
	//courseModel_.reset();
	/*course_ = std::make_unique<Course>();
	ModelManager::GetInstance()->AppendModel(CourseLoader::LoadCourseFile("Resources/Course", "courseWallTest2.course", *(course_->GetCoursePolygonsAdress())));
	LevelData::MeshData courseData;
	courseData.directoryPath = "Resources/Course";
	courseData.flieName = "courseWallTest2.course";
	courseData.transform = { 1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	courseData.className = "Course";
	courseData.name = "";
	courseData.parentName = "";
	course_->Initialize(&courseData);
	*/
	isDebugCameraActive_ = true;

	// モデル描画
	ModelDraw::PreDrawParameters preDrawParameters;
	preDrawParameters.directionalLight = directionalLight_.get();
	preDrawParameters.fogManager = FogManager::GetInstance();
	preDrawParameters.pointLightManager = pointLightManager_.get();
	preDrawParameters.spotLightManager = spotLightManager_.get();
	preDrawParameters.environmentTextureHandle = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());
	ModelDraw::SetPreDrawParameters(preDrawParameters);

	// 
	courseCollisionSystem_ = std::make_unique<CourseCollisionSystem>();
	courseCollisionSystem_->Initialize();
	//courseCollisionSystem_->SetCourse(course_.get());

	// コースデモ用
	courseDemoModel_.reset(Model::Create("Resources/default/", "ball.obj", dxCommon_));
	courseDemoObject_ = std::make_unique<CourseDemoObject>();
	LevelData::MeshData courseDemoData;
	courseDemoData.directoryPath = "Resources/default/";
	courseDemoData.flieName = "ball.obj";
	courseDemoData.transform = { 1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	courseDemoData.className = "CourseDemo";
	courseDemoData.name = "";
	courseDemoData.parentName = "";
	courseDemoObject_->Initialize(&courseDemoData);

	// ミニガン
	objModel_.reset(Model::Create("Resources/Model/Gimmick/Cannon/", "Cannon.obj", dxCommon_));
	objG_ = std::make_unique<Cannon>();
	LevelData::MeshData objData;
	objData.directoryPath = "Resources/Model/Gimmick/Cannon/";
	objData.flieName = "Cannon.obj";
	objData.transform = { 1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	objData.className = "Obstacle";
	objData.name = "";
	objData.parentName = "";
	OBB objCollider;
	objCollider.center_ = { 0.0f,0.0f,0.0f };
	objCollider.size_ = { 1.0f,1.0f,1.0f };
	objCollider.SetOtientatuons(Matrix4x4::MakeIdentity4x4());
	objData.collider = objCollider;
	objG_->Initialize(&objData);

	BaseScene::InitilaizeCheck();

}

void DebugScene::Update()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RSHIFT)) {
		// 行きたいシーンへ
		requestSceneNo_ = kGame;
	}
#endif // _DEBUG

	//clothDemo_->Update();

	ParticleManager_->Update();

	UIManager_->Update();
	// オブジェクトマネージャー
	objectManager_->Update();

	courseDemoObject_->Update();
	//courseCollisionSystem_->ObjectRegistration(courseDemoObject_.get());
	//courseCollisionSystem_->Execute();
	
	objG_->Update();

	DebugCameraUpdate();

	ImguiDraw();

	ImGui::Begin("ParticleManagerStopEmissionTest");
	if (ImGui::Button("Button")) {
		
	}
	ImGui::End();
ParticleManager_->StopEmission("RunDust");
}

void DebugScene::Draw()
{

	ModelDraw::PreDraw(dxCommon_->GetCommadList());

	// スカイドーム
	skydome_->Draw(camera_);

	objectManager_->Draw(camera_, drawLine_);

	//コース表示
	//course_->Draw(camera_);

	// コースデモ
	//courseDemoObject_->Draw(camera_);

	objG_->Draw(camera_);

	ModelDraw::PostDraw();

#pragma region 線描画

	drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

	//drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

	//clothDemo_->Draw(&camera_);

#pragma endregion

	ParticleManager_->Draw(camera_);

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景
	//前景スプライト描画

	// UI
	UIManager_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void DebugScene::ImguiDraw()
{

	debugCamera_->ImGuiDraw();

	course_->ImGuiDraw();
	//clothDemo_->ImGuiDraw(camera_);

	//courseCollisionSystem_->ImGuiDraw();

	UIManager_->ImGuiDraw();

}

void DebugScene::DebugCameraUpdate()
{

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}

}
