#include "DriveSystem.h"
#include "../../VehicleCore.h"
#include "../../../Utility/Calc/TransformHelper.h"
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

void DriveSystem::Initialize()
{
	handling_->SetOwner(owner_);
}

void DriveSystem::Update()
{
	// ハンドル処理
	handling_->Update();
	// エンジン処理
	driveEngine_->Update();
	// 速度の計算
	Vector3 acc = Vector3::FrontVector() * driveEngine_->GetCurrentSpeed();
	velocity_ += acc * kDeltaTime_;
	// 減速
	const float velocityDecrement = 0.75f;
	const float kEpsilon = 0.001f;
	velocity_ = velocity_ * velocityDecrement;
	// 0に調節
	VehicleCaluclator calc;
	velocity_ = calc.SnapToZero(velocity_, kEpsilon);
	// 速度が無ければ
	if (velocity_ == Vector3::FrontVector()) {
		return;
	}
	// 角度
	float eulerY = TransformHelper::CalculateXZVectorToRotateRadian(owner_->GetWorldTransformAdress()->direction_, Vector3::FrontVector());
	coreTransform_->transform_.translate += 
		calc.RotateVector(velocity_, eulerY) * kDeltaTime_;
}

void DriveSystem::InputAccept(GameKeyconfig* keyConfig)
{
	// エンジンの受付
	driveEngine_->EngineAccept(keyConfig);
	// ハンドルの入力受付
	handling_->HandleInput(keyConfig->GetLeftStick()->x);
}

void DriveSystem::ImGuiDraw()
{
	// 速度
	ImGui::DragFloat3("Velocity", &velocity_.x);
	// ハンドル
	handling_->ImGuiDraw();
	// エンジン
	driveEngine_->ImGuiDraw();
}
