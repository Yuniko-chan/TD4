#include "DriveEngine.h"
#include "../VehicleSystems.h"
#include "../../VehicleCore.h"

#include "../../../Player/DebugData/PlayerDebugData.h"

void DriveEngine::Update()
{
	// フレームカウント
	const int timming = 10;
	const int maxReception = 10;

	// アクセルキーか受付連続値があれば
	if ((isAccel_ || isDecel_) || consecutiveReceptions_ != 0) {
		inputCounter_++;
	}

	// 加速減速（タイヤが無ければ加速減速の処理を受け付けない）
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

	// 速度処理
	SpeedCalculation();
}

void DriveEngine::Reset()
{
	// 初期化
	consecutiveReceptions_ = 0;

}

void DriveEngine::EngineAccept(GameKeyconfig* keyConfig)
{
		
	isAccel_ = keyConfig->GetConfig()->accel;
	isDecel_ = keyConfig->GetConfig()->brake;

	// タイヤの数
	int tireCount = owner_->GetStatus()->GetTire();
	// タイヤがなければ入力を削除
	if (tireCount <= 0) {
		isAccel_ = false;
		isDecel_ = false;
	}

}

void DriveEngine::ImGuiDraw()
{

	if (ImGui::TreeNode("EngineInfo")) {
		ImGui::Checkbox("IsAccel", &isAccel_);
		ImGui::Checkbox("IsDecel", &isDecel_);
		int con = this->consecutiveReceptions_;
		ImGui::InputInt("ConsecutiveRecept", &con);
		ImGui::DragFloat("SpeedRatio", &speedRate_);
		ImGui::InputFloat("CurrentSpeed", &currentSpeed_);

		ImGui::TreePop();
	}

}

void DriveEngine::SpeedCalculation()
{
	// スピード用のレシオ計算
	const float kMaxRate = 10.0f;	// 最大
	const float kMinRate = 1.0f;	// 最小
	const float kEngineMax = 10.0f;	// エンジンの最大数
	// レート
	float engineCount = (float)owner_->GetStatus()->GetEngine();
	float t = (std::clamp(engineCount, 0.0f, 9.0f) + 1.0f) / kEngineMax;
	// 乗算レート
	float plusRate = Ease::Easing(Ease::EaseName::Lerp, kMinRate, kMaxRate, t);
	// エンジンが回転している場合
	if (consecutiveReceptions_ != 0) {
		// 速度計算
		speedRate_ = (float)consecutiveReceptions_ * (plusRate);

		// 全体への影響（速度レートが一定を越えている場合オーバーヒート的な何か）
		OverheatProcess(t);

	}
	// エンジンが回転していない場合
	else {
		// 速度が残っている場合
		if (speedRate_ != 0.0f) {
			const float decreValue = 0.05f;
			speedRate_ = Ease::Easing(Ease::EaseName::Lerp, speedRate_, 0.0f, decreValue);
		}
	}

	// 運転されていなければ
	if (!owner_->IsDrive()) {
		// 速度が残っている場合
		if (speedRate_ != 0.0f) {
			const float decreValue = 0.1f;
			speedRate_ = Ease::Easing(Ease::EaseName::Lerp, speedRate_, 0.0f, decreValue);
		}
	}

	// 加速度の計算
	const float rideSpeedFactor = GlobalVariables::GetInstance()->GetFloatValue("Player", "RideSpeed");

	// 速度があるとき
	if (speedRate_ != 0.0f) {
		currentSpeed_ = speedRate_ * rideSpeedFactor;
	}

	// 切り捨て
	const float discard = 0.001f;
	if (std::fabsf(currentSpeed_) <= discard) {
		currentSpeed_ = 0.0f;
	}
}

void DriveEngine::OverheatProcess(const float& SpeedPercentage)
{
	// 押し込みが半分以上かつ
	const float speedLimit = 0.5f;
	const float receptionLimit = 10 / 2;
	if (SpeedPercentage >= speedLimit &&
		std::abs(consecutiveReceptions_) >= receptionLimit) {
		this->owner_->GetStatus()->SetIsOverheat(true);
		float minDPS = 1.0f;
		float maxDPS = 7.5f;
		// スピード用のレシオ計算
		const float kMaxRate = 10.0f;	// 最大
		const float kMinRate = 1.0f;	// 最小
		const float kEngineMax = 10.0f;	// エンジンの最大数
		// レート
		float engineCount = (float)owner_->GetStatus()->GetEngine();
		float t = (std::clamp(engineCount, 0.0f, 9.0f) + 1.0f) / kEngineMax;
		// 乗算レート
		float plusRate = Ease::Easing(Ease::EaseName::Lerp, kMinRate, kMaxRate, t);
		float maxRate = receptionLimit * plusRate;
		t = (std::fabsf(speedRate_) - (maxRate)) / maxRate;

		float dps = Ease::Easing(Ease::EaseName::Lerp, minDPS, maxDPS, t);

		// SpeedRate(now / max) = t
		// Ease(minDPS,maxDPS,t)
		owner_->GetStatus()->SetDamagePerSecond(std::fabsf(dps));
	}


}
