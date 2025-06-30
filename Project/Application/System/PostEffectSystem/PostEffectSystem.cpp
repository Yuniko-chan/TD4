#include "PostEffectSystem.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"
#include "../../Object/Player/State/User/PlayerInVehicleState.h"
#include "../../Object/Utility/Calc/VehicleCaluclator.h"

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

	// 放射状ブラーの広がる最大の強さ
	radialBlurStrengthMax_ = 0.5f;

	// 放射状ブラーがかかる速度
	radialBlurSpeed_ = 200.0f;

	// 放射状ブラーが最大でかかる速度
	radialBlurSpeedMax_ = 420.0f;

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
	if (driveSystem_ && player_) {

		// 型
		const std::type_info& currentState = typeid(*(player_->GetStateMachine()->GetCurrentState()));
		std::string name = currentState.name();
		bool playerDriving = name == "class PlayerInVehicleState";
		float driveSpeed = driveSystem_->GetVelocity().z;
;		if (driveSpeed >= radialBlurSpeed_ && playerDriving) {
			runRadialBlur_ = true;
			float t = 0.0f;
			if (!((radialBlurSpeedMax_ - radialBlurSpeed_) <= 0.0f)) {
				VehicleCaluclator calc;
				t = calc.BlurStrengthFactor(driveSystem_, radialBlurSpeed_, radialBlurSpeedMax_);
			}
			float radialBlurStrength = Ease::Easing(Ease::EaseName::EaseOutCubic, radialBlurStrength_, radialBlurStrengthMax_, t);
			postEffect_->SetRadialBlurStrength(radialBlurStrength);
		}
		else {
			runRadialBlur_ = false;
		}
	}
	else {
		runRadialBlur_ = false;
	}

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

	radialBlurStrengthMax_ = globalVariables->GetFloatValue(groupName, "radialBlurStrengthMax");
	
	radialBlurSpeed_ = globalVariables->GetFloatValue(groupName, "radialBlurSpeed");

	radialBlurSpeedMax_ = globalVariables->GetFloatValue(groupName, "radialBlurSpeedMax");

}

void PostEffectSystem::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "PostEffectSystem";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "radialBlurStrength", radialBlurStrength_);

	globalVariables->AddItem(groupName, "radialBlurStrengthMax", radialBlurStrengthMax_);
	
	globalVariables->AddItem(groupName, "radialBlurSpeed", radialBlurSpeed_);

	globalVariables->AddItem(groupName, "radialBlurSpeedMax", radialBlurSpeedMax_);

}
