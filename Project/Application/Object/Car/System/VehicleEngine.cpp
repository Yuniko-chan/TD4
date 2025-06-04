#include "VehicleEngine.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/3D/Transform/WorldTransform.h"

#include "../../Player/DebugData/PlayerDebugData.h"

void VehicleEngine::Update()
{
	// フレームカウント
	const int timming = 10;
	const int maxReception = 10;

	// アクセルキーか受付連続値があれば
	if ((isAccel_ || isDecel_) || consecutiveReceptions_ != 0) {
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
	else if (!(isAccel_ || isDecel_) && (inputCounter_ % (timming / 2) == 0)) {
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
	
	// 加速度の計算
	const float rideSpeedFactor = GlobalVariables::GetInstance()->GetFloatValue("Player", "RideSpeed");
	currentSpeed_ = speedRatio_ * rideSpeedFactor;
	// 切り捨て
	const float discard = 0.001f;
	if (std::fabsf(currentSpeed_) <= discard) {
		currentSpeed_ = 0.0f;
	}

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
	
}

void VehicleEngine::ImGuiDraw()
{

	if (ImGui::TreeNode("EngineInfo")) {
		ImGui::Checkbox("IsAccel", &isAccel_);
		ImGui::Checkbox("IsDecel", &isDecel_);
		int con = this->consecutiveReceptions_;
		ImGui::InputInt("ConsecutiveRecept", &con);
		ImGui::DragFloat("SpeedRatio", &speedRatio_);

		ImGui::TreePop();
	}

}
