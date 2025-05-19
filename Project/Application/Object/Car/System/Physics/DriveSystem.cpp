#include "DriveSystem.h"
#include "../../VehicleCore.h"
#include "../../../KeyConfig/GameKeyconfig.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/2D/ImguiManager.h"

DriveSystem::DriveSystem()
{
	// エンジン
	driveEngine_ = std::make_unique<VehicleEngine>();
}

void DriveSystem::Update()
{
	// エンジン処理
	driveEngine_->Update();

	// 速度の計算
	velocity_ += driveEngine_->GetAcceleration() * kDeltaTime_;
	// 減速
	velocity_ = velocity_ * 0.75f;
	const float kEpsilon = 0.0001f;
	if (std::fabsf(velocity_.x) < kEpsilon) velocity_.x = 0.0f;
	if (std::fabsf(velocity_.y) < kEpsilon) velocity_.y = 0.0f;
	if (std::fabsf(velocity_.z) < kEpsilon) velocity_.z = 0.0f;


	if (velocity_ != Vector3(0.0f, 0.0f, 0.0f)) {
		Vector3 direct = Vector3::Normalize(velocity_);
		coreTransform_->transform_.rotate.y = std::atan2f(direct.x, direct.z);
	}
	else {
		coreTransform_->transform_.rotate.y = 0.0f;
	}
	
	// 移動計算
	coreTransform_->transform_.translate += velocity_ * kDeltaTime_;
	//coreTransform_->transform_.rotate.y = driveEngine_->GetEuler();
	// 初期化
	driveEngine_->SetDirection(Vector3(0.0f, 0.0f, 0.0f));
}

void DriveSystem::InputAccept(GameKeyconfig* keyConfig, const Vector3& direct)
{
	// エンジンの設定
	driveEngine_->SetDirection(direct);
	// エンジンの受付
	driveEngine_->EngineAccept(keyConfig);
}

void DriveSystem::ImGuiDraw()
{
	// 速度
	ImGui::DragFloat3("Velocity", &velocity_.x);
	// エンジン
	driveEngine_->ImGuiDraw();
}
