#include "DriveSystem.h"
#include "../../VehicleCore.h"
#include "../../../KeyConfig/GameKeyconfig.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/2D/ImguiManager.h"

DriveSystem::DriveSystem()
{
	// エンジン
	driveEngine_ = std::make_unique<VehicleEngine>();
	// ハンドル
	handling_ = std::make_unique<VehicleHandling>();
}

void DriveSystem::Update()
{
	// ハンドル処理
	handling_->Update();
	// エンジン処理
	driveEngine_->Update(handling_->GetSteerDirect());

	// 速度の計算
	velocity_ += driveEngine_->GetAcceleration() * kDeltaTime_;
	// 減速
	const float velocityDecrement = 0.75f;
	velocity_ = velocity_ * velocityDecrement;
	const float kEpsilon = 0.0001f;
	// 0に調節
	VehicleCaluclator calc;
	velocity_ = calc.SnapToZero(velocity_, kEpsilon);

	if (velocity_ != Vector3(0.0f, 0.0f, 0.0f)) {
		Vector3 direct = Vector3::Normalize(velocity_);
		coreTransform_->transform_.rotate.y = std::atan2f(direct.x, direct.z);
	}
	else {
		coreTransform_->transform_.rotate.y = 0.0f;
	}
	
	// 移動計算
	coreTransform_->transform_.translate += 
		calc.RotateVector(velocity_,coreTransform_->transform_.rotate.y) * kDeltaTime_;
}

void DriveSystem::InputAccept(GameKeyconfig* keyConfig, const Vector3& direct)
{
	// エンジンの設定
	driveEngine_->SetDirection(direct);
	// エンジンの受付
	driveEngine_->EngineAccept(keyConfig);
	// ハンドルの入力受付
	handling_->HandleInput(keyConfig->GetLeftStick()->x);
}

void DriveSystem::ImGuiDraw()
{
	// 速度
	ImGui::DragFloat3("Velocity", &velocity_.x);
	// エンジン
	driveEngine_->ImGuiDraw();
}
