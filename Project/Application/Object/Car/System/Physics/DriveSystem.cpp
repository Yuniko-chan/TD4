#include "DriveSystem.h"
#include "../../VehicleCore.h"
#include "../../../KeyConfig/GameKeyconfig.h"
#include "../../../Engine/Math/DeltaTime.h"

DriveSystem::DriveSystem()
{
	// エンジン
	driveEngine_ = std::make_unique<VehicleEngine>();
}

void DriveSystem::Update()
{
	// エンジン処理
	driveEngine_->Update();



	// 移動計算
	coreTransform_->transform_.translate += driveEngine_->GetDirection() * kDeltaTime_;

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
