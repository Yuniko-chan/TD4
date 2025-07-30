#include "TitleScene.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../Object/Manager/TitleSceneObjectManager.h"
#include "../../Object/Factory/ObjectFactory.h"

TitleScene::~TitleScene()
{

	if (stopAudio_) {
		for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
			audioManager_->StopWave(i);
		}
	}

}

void TitleScene::Initialize()
{

	BaseScene::Initialize();

	ModelCreate();
	TextureLoad();

	// オーディオマネージャー
	audioManager_ = std::make_unique<TitleAudioManager>();
	audioManager_->StaticInitialize();
	audioManager_->Initialize();

	// 一度鳴らして止める
	for (uint32_t i = 0; i < TitleAudioNameIndex::kTitleAudioNameIndexOfCount; ++i) {
		audioManager_->PlayWave(i);
	}
	for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
		audioManager_->StopWave(i);
	}

	// オブジェクトマネージャー
	objectManager_ = std::make_unique<TitleSceneObjectManager>();
	objectManager_->Initialize(kLevelIndexTitle, levelDataManager_);

	const EulerTransform cameraTransform = {
		1.0f,1.0f,1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -10.0f};

	camera_.SetTransform(cameraTransform);
	camera_.Update();

	titleSpriteObjects_ = std::make_unique<TitleSpriteObjects>();
	titleSpriteObjects_->Initialize();


	audioManager_->PlayWave(kTitleBGM);

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

void TitleScene::Update()
{

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		// 行きたいシーンへ
		requestSceneNo_ = kGame;
		audioManager_->PlayWave(kTitlePushButton);
	}
	if (input_->TriggerKey(DIK_F2)) {
		// 行きたいシーンへ
		requestSceneNo_ = kGame;
		audioManager_->PlayWave(kTitlePushButton);
	}

	objectManager_->Update();

	titleSpriteObjects_->Update();

	// デバッグカメラ
	DebugCameraUpdate();

	ImguiDraw();

}

void TitleScene::Draw()
{

#pragma region モデル描画

	ModelDraw::PreDraw(dxCommon_->GetCommadList());

	//3Dオブジェクトはここ

	objectManager_->Draw(camera_, drawLine_);

	ModelDraw::PostDraw();

#pragma endregion

	// パーティクル描画
	objectManager_->ParticleDraw(camera_);

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	titleSpriteObjects_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

	// 深度バッファクリア
	renderTargetTexture_->ClearDepthBuffer();

}

void TitleScene::ImguiDraw()
{

#ifdef _DEMO

	PostEffect::GetInstance()->ImGuiDraw();

	debugCamera_->ImGuiDraw();

#endif // _DEMO

}

void TitleScene::DebugCameraUpdate()
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

void TitleScene::ModelCreate()
{

}

void TitleScene::TextureLoad()
{

}
