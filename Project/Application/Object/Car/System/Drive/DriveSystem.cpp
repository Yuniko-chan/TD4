#include "DriveSystem.h"
#include "../../VehicleCore.h"
#include "../../../Utility/Calc/TransformHelper.h"
#include "../../../KeyConfig/GameKeyconfig.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/2D/ImguiManager.h"

DriveSystem::DriveSystem()
{
	// エンジン
	driveEngine_ = std::make_unique<DriveEngine>();
	// ハンドル
	handling_ = std::make_unique<DriveHandling>();
}

void DriveSystem::Initialize()
{
	// ハンドルクラス
	handling_->SetOwner(owner_);

	// エンジングラス
	driveEngine_->SetOwner(owner_);
}

void DriveSystem::Update()
{
	//---それぞれのシステム処理---//
	// ハンドル処理
	handling_->SetVehicleDirection(owner_->GetWorldTransformAdress()->direction_);
	handling_->PreUpdate();
	// エンジン処理
	driveEngine_->Update();
	// 速度処理
	VelocityUpdate();

	//---角度の設定---//
	// ハンドル操作の更新（旋回の適応など）
	handling_->PostUpdate(velocity_, status_);

	// 速度が無ければ早期
	if (velocity_ == Vector3(0.0f, 0.0f, 0.0f))
	{
		return;
	}
	// 角度
	//float eulerY = TransformHelper::CalculateXZVectorToRotateRadian(owner_->GetWorldTransformAdress()->direction_, Vector3::FrontVector());
	Vector3 newDirect = Matrix4x4::TransformNormal(velocity_, Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), owner_->GetWorldTransformAdress()->direction_));
	
	// 座標計算
	//VehicleCaluclator calc;
	owner_->GetWorldTransformAdress()->transform_.translate += newDirect * kDeltaTime_;

}

void DriveSystem::PreUpdate()
{

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

void DriveSystem::VelocityUpdate()
{
	//---速度の設定---//
	// 速度の計算
	// 速度レートが0の場合加算しない
	if (driveEngine_->GetCurrentSpeed() != 0) {
		Vector3 acceleration = Vector3::FrontVector() * driveEngine_->GetCurrentSpeed();
		velocity_ += acceleration * kDeltaTime_;
	}

	const float velocityDecrement = 0.75f;	// 減速値
	const float kEpsilon = 0.001f;	// 切り捨て値
	// 減速
	velocity_ = velocity_ * velocityDecrement;
	// 0に調節
	VehicleCaluclator calc;
	velocity_ = calc.SnapToZero(velocity_, kEpsilon);

}
