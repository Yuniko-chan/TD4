#include "DebugScene.h"

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

	// コース
	courseModel_.reset(Model::Create("Resources/default/", "plane.obj", dxCommon_));
	course_ = std::make_unique<Course>();
	LevelData::MeshData courseData;
	courseData.directoryPath = "Resources/default/";
	courseData.flieName = "plane.obj";
	courseData.transform = { 1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	courseData.className = "Course";
	courseData.name = "";
	courseData.parentName = "";
	course_->Initialize(&courseData);

	clothDemo_ = std::make_unique<ClothDemo>();
	clothDemo_->Initilalize(directionalLight_.get(), pointLightManager_.get(), spotLightManager_.get());

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

	BaseScene::InitilaizeCheck();

}

void DebugScene::Update()
{

	clothDemo_->Update();

	courseDemoObject_->Update();
	courseCollisionSystem_->ObjectRegistration(courseDemoObject_.get());
	courseCollisionSystem_->Execute();

	DebugCameraUpdate();

	ImguiDraw();

}

void DebugScene::Draw()
{

	ModelDraw::PreDraw(dxCommon_->GetCommadList());

	// スカイドーム
	skydome_->Draw(camera_);

	// コースデモ
	courseDemoObject_->Draw(camera_);

	course_->Draw(camera_);

	clothDemo_->CollisionObjectDraw(&camera_);

	ModelDraw::PostDraw();

#pragma region 線描画


	//drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

	clothDemo_->Draw(&camera_);

#pragma endregion

}

void DebugScene::ImguiDraw()
{

	clothDemo_->ImGuiDraw(camera_);

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
