#include "ClearScene.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"

ClearScene::~ClearScene()
{

}

void ClearScene::Initialize()
{

	BaseScene::Initialize();

	ModelCreate();
	TextureLoad();

	const EulerTransform cameraTransform = {
	1.0f,1.0f,1.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -10.0f };

	camera_.SetTransform(cameraTransform);
	camera_.Update();

	// モデル描画
	ModelDraw::PreDrawParameters preDrawParameters;
	preDrawParameters.directionalLight = directionalLight_.get();
	preDrawParameters.fogManager = FogManager::GetInstance();
	preDrawParameters.pointLightManager = pointLightManager_.get();
	preDrawParameters.spotLightManager = spotLightManager_.get();
	preDrawParameters.environmentTextureHandle = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());
	ModelDraw::SetPreDrawParameters(preDrawParameters);

	// コース踏破システム
	courseTraversalSystem_ = std::make_unique<CourseTraversalSystem>();
	courseTraversalSystem_->Initialize();

}

void ClearScene::Update()
{

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		// 行きたいシーンへ
		requestSceneNo_ = kTitle;
	}

	courseTraversalSystem_->Update();

	ImguiDraw();
	
}

void ClearScene::Draw()
{

#pragma region モデル描画

	ModelDraw::PreDraw(dxCommon_->GetCommadList());

	ModelDraw::PostDraw();

#pragma endregion

#pragma region スプライト描画
	// スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景
	//前景スプライト描画
	courseTraversalSystem_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void ClearScene::ImguiDraw()
{

#ifdef _DEMO
	courseTraversalSystem_->ImGuiDraw();
#endif // _DEMO

}

void ClearScene::ModelCreate()
{

}

void ClearScene::TextureLoad()
{

}
