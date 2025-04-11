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

}

void TitleScene::Initialize()
{

	BaseScene::Initialize();

	ModelCreate();
	TextureLoad();

	// オブジェクトマネージャー
	objectManager_ = std::make_unique<TitleSceneObjectManager>();
	objectManager_->Initialize(kLevelIndexTitle, levelDataManager_);

	const EulerTransform cameraTransform = {
		1.0f,1.0f,1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -10.0f};

	camera_.SetTransform(cameraTransform);
	camera_.Update();

	// ボタンスプライト位置
	const Vector2 kButtonSpritePosition = { 400.0f, 540.0f };
	buttonColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	buttonSprite_.reset(Sprite::Create(buttonTextureHandle_, kButtonSpritePosition, buttonColor_));
	buttonAlphaT_ = 0.0f;
	// α値変更速度
	const float kButtonAlphaTSpeed = 0.01f;
	buttonAlphaTSpeed_ = kButtonAlphaTSpeed;
	buttonItIncreaseAlphaT_ = true;

	// タイトル背景
	titleBackGround_ = std::make_unique<TitleBackGround>();
	titleBackGround_->Initialize();

	//「スタート」スプライト
	const Vector2 kStartSpritePosition = { 640.0f, 540.0f };
	startSprite_.reset(Sprite::Create(startTextureHandle_, kStartSpritePosition, { 1.0f, 1.0f, 1.0f, 1.0f }));

	// ロゴスプライト
	const Vector2 kLogoSpritePosition = { 640.0f, 200.0f };
	logoSprite_.reset(Sprite::Create(logoTextureHandle_, kLogoSpritePosition, { 1.0f, 1.0f, 1.0f, 1.0f }));

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
	}

	objectManager_->Update();

	// デバッグカメラ
	DebugCameraUpdate();

	// ボタンスプライト
	if (buttonItIncreaseAlphaT_) {
		buttonAlphaT_ += buttonAlphaTSpeed_;
		if (buttonAlphaT_ >= 1.0f) {
			buttonAlphaT_ = 1.0f;
			buttonItIncreaseAlphaT_ = false;
		}
	}
	else {
		buttonAlphaT_ -= buttonAlphaTSpeed_;
		if (buttonAlphaT_ <= 0.0f) {
			buttonAlphaT_ = 0.0f;
			buttonItIncreaseAlphaT_ = true;
		}
	}
	buttonColor_.w = Ease::Easing(Ease::EaseName::EaseInOutQuad, 0.0f, 1.0f, buttonAlphaT_);
	buttonSprite_->SetColor(buttonColor_);

	// タイトル背景
	titleBackGround_->Update();

	ImguiDraw();

}

void TitleScene::Draw()
{

#pragma region モデル描画

	ModelDraw::PreDraw(dxCommon_->GetCommadList());

	//3Dオブジェクトはここ

	objectManager_->Draw(camera_, drawLine_);

	// タイトル背景
	titleBackGround_->Draw(camera_);

	ModelDraw::PostDraw();

#pragma endregion

	// パーティクル描画
	objectManager_->ParticleDraw(camera_);

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// ボタン
	buttonSprite_->Draw();

	// 「スタート」
	startSprite_->Draw();

	// ロゴ
	logoSprite_->Draw();

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

	buttonTextureHandle_ = TextureManager::Load("Resources/UI/ButtonA.png", dxCommon_);
	startTextureHandle_ = TextureManager::Load("Resources/OutGame/Start.png", dxCommon_);
	logoTextureHandle_ = TextureManager::Load("Resources/OutGame/TitleLogo.png", dxCommon_);

}
