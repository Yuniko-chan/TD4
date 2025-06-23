#include "PostEffectSystem.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void PostEffectSystem::Initialize()
{

	// ポストエフェクト
	postEffect_ = PostEffect::GetInstance();
	// DirectXCommon
	dxCommon_ = DirectXCommon::GetInstance();
	// ウインドウスプライト
	windowSprite_ = WindowSprite::GetInstance();

	// レンダーターゲットテクスチャ
	renderTargetTexture_ = nullptr;

	// ラジアルブラー実行するか
	runRadialBlur_ = false;

	// 放射状ブラーの広がる強さ
	radialBlurStrength_ = 0.2f;

	// グローバル変数を登録する
	RegisteringGlobalVariables();

	// グローバル変数を適用する
	ApplyGlobalVariables();

}

void PostEffectSystem::Update()
{

#ifdef _DEMO
	ApplyGlobalVariables();
#endif // _DEMO

	// ラジアルブラー
	postEffect_->SetRadialBlurStrength(radialBlurStrength_);

}

void PostEffectSystem::Execute()
{

	// ラジアルブラー
	if (runRadialBlur_) {
		postEffect_->Execution(
			dxCommon_->GetCommadList(),
			renderTargetTexture_,
			PostEffect::kCommandIndexRadialBlur
		);
		windowSprite_->DrawSRV(PostEffect::GetInstance()->GetEditTextures(0));
	}

}

void PostEffectSystem::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "PostEffectSystem";

	radialBlurStrength_ = globalVariables->GetFloatValue(groupName, "radialBlurStrength");

}

void PostEffectSystem::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "PostEffectSystem";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "radialBlurStrength", radialBlurStrength_);

}
