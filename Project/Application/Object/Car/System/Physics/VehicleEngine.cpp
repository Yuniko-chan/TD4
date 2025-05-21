#include "VehicleEngine.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/3D/Transform/WorldTransform.h"

#include "../../../Player/DebugData/PlayerDebugData.h"

void VehicleEngine::Update(const Vector3& steerDirect)
{
	// フレームカウント
	const int timming = 10;
	const int maxReception = 10;

	// アクセルキーか受付連続値があれば
	if ((isAccel_ || isDecel_) || consecutiveReceptions_ != 0) {
		accelInputCounter_++;

		inputCounter_++;
	}

	// 加速減速
	if (inputCounter_ % timming == 0) {
		if (isAccel_) {
			consecutiveReceptions_++;
		}
		else if (isDecel_) {
			consecutiveReceptions_--;
		}

		inputCounter_ = 0;
	}
	else if (!(isAccel_ || isDecel_) && (inputCounter_ % 5 == 0)) {
		if (consecutiveReceptions_ > 0) {
			consecutiveReceptions_--;
		}
		else if (consecutiveReceptions_ < 0) {
			consecutiveReceptions_++;
		}
		
		inputCounter_ = 0;
	}

	// 制限処理
	consecutiveReceptions_ = (int16_t)std::clamp((int)consecutiveReceptions_, -maxReception, maxReception);

	// スピード用のレシオ計算
	const float kPlusRate = 3.0f;
	// エンジンが回転している場合
	if (consecutiveReceptions_ != 0) {
		speedRatio_ = (float)consecutiveReceptions_ * (kPlusRate);
	}
	// エンジンが回転していない場合
	else {
		// 速度が残っている場合
		if (speedRatio_ != 0.0f) {
			const float decreValue = 0.05f;
			speedRatio_ = Ease::Easing(Ease::EaseName::Lerp, speedRatio_, 0.0f, decreValue);
		}
	}
	
	// 向きに併せるためのベクトル
	Vector3 newDirection = Vector3::Normalize(steerDirect);
	// 加速度の計算
	const float rideSpeedFactor = GlobalVariables::GetInstance()->GetFloatValue("Player", "RideSpeed");
	acceleration_ = newDirection * (speedRatio_ * rideSpeedFactor);

}

void VehicleEngine::Reset()
{
	// 初期化
	consecutiveReceptions_ = 0;

}

void VehicleEngine::EngineAccept(GameKeyconfig* keyConfig)
{
	isAccel_ = keyConfig->GetConfig()->accel;
	isDecel_ = keyConfig->GetConfig()->brake;
	
	controlDirect_ = keyConfig->GetLeftStick()->x;

	handringDirect_.x = Ease::Easing(Ease::EaseName::Lerp, -1.0f, 1.0f, (controlDirect_ + 1.0f) / 2.0f);
	handringDirect_.z = 1.0f;
	handringDirect_ = Vector3::Normalize(handringDirect_);
}

void VehicleEngine::ImGuiDraw()
{
	ImGui::Checkbox("IsAccel", &isAccel_);
	ImGui::Checkbox("IsDecel", &isDecel_);

	int con = this->accelInputCounter_;
	ImGui::InputInt("AccelInput", &con);
	con = this->decelInputCounter_;
	ImGui::InputInt("DecelInput", &con);
	con = this->consecutiveReceptions_;
	ImGui::InputInt("ConsecutiveRecept", &con);
	ImGui::DragFloat("SpeedRatio", &speedRatio_);
	// ハンドル角
	ImGui::DragFloat("LeftstickX", &controlDirect_);
	ImGui::DragFloat3("Handle", &handringDirect_.x);
}
