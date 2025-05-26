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

	// オブジェクトマネージャー
	objectManager_ = std::make_unique<GameSceneObjectManager>();
	objectManager_->Initialize(kLevelIndexDebug, levelDataManager_);

	// コース
	//courseModel_.reset();
	course_ = std::make_unique<Course>();
	ModelManager::GetInstance()->AppendModel(CourseLoader::LoadCourseFile("Resources/Course", "courseWallTest2.course", *(course_->GetCoursePolygonsAdress())));
	LevelData::MeshData courseData;
	courseData.directoryPath = "Resources/Course";
	courseData.flieName = "courseWallTest2.course";
	courseData.transform = { 1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	courseData.className = "Course";
	courseData.name = "";
	courseData.parentName = "";
	course_->Initialize(&courseData);
	
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
	courseCollisionSystem_->SetCourse(course_.get());

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

	// オブジェクトマネージャー
	objectManager_->Update();

	courseDemoObject_->Update();
	courseCollisionSystem_->ObjectRegistration(courseDemoObject_.get());
	courseCollisionSystem_->Execute();
	
	objG_->Update();

	DebugCameraUpdate();

	ImguiDraw();

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

#pragma endregion

}

void DebugScene::ImguiDraw()
{

	debugCamera_->ImGuiDraw();

	course_->ImGuiDraw();

	courseCollisionSystem_->ImGuiDraw();

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
