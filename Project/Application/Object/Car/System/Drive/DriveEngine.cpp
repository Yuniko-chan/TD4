#include "DriveEngine.h"
#include "../VehicleSystems.h"
#include "../../VehicleCore.h"

#include "../../../Player/DebugData/PlayerDebugData.h"
#include "../../../GameTimer/GameTimeSystem.h"

static int sTimming = 10;	// 入力が加算されるタイミング
static int sMaxReception = 10;	// 受付最大数

static float sMaxRate = 10.0f;	// 最大
static float sMinRate = 1.0f;	// 最小
static float sEngineMax = 10.0f;	// エンジンの最大数

static float sMinDPS = 1.0f;	// 最小ダメージ
static float sMaxDPS = 7.5f;	// 最大ダメージ

void DriveEngine::Update()
{
	// リセット呼び出し
	if (!owner_->IsPlayer() && !onReset_) {
		onReset_ = std::bind(&DriveEngine::Reset, this);
		onReset_.value()();
	}
	// プレイヤーに乗られてなければ
	else if (owner_->IsPlayer() && onReset_) {
		onReset_ = std::nullopt;
	}

	// フレームカウント（時間を基に）
	float timming = sTimming * kDeltaTime_;
	float decrementTimming = (sTimming / 2.0f) * kDeltaTime_;
	int maxReception = sMaxReception;

	// アクセルキーか受付連続値があれば
	if ((isAccel_ || isDecel_) || consecutiveReceptions_ != 0) {
		inputCounter_ += kDeltaTime_;;
	}

	// 加速減速（タイヤが無ければ加速減速の処理を受け付けない）
	if (inputCounter_ >= timming) {
		if (isAccel_) {
			consecutiveReceptions_++;
		}
		else if (isDecel_) {
			consecutiveReceptions_--;
		}

		inputCounter_ = 0.0f;
	}
	else if (!(isAccel_ || isDecel_) && (inputCounter_ >= decrementTimming)) {
		if (consecutiveReceptions_ > 0) {
			consecutiveReceptions_--;
		}
		else if (consecutiveReceptions_ < 0) {
			consecutiveReceptions_++;
		}
		
		inputCounter_ = 0.0f;
	}

	// 制限処理
	consecutiveReceptions_ = (int16_t)std::clamp((int)consecutiveReceptions_, -maxReception, maxReception);

	// 速度処理
	SpeedCalculation();
}

void DriveEngine::Reset()
{
	// 初期化
	//consecutiveReceptions_ = 0;
	isAccel_ = false;
	isDecel_ = false;

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
	ImGui::SeparatorText("ドライブエンジン");



	if (ImGui::TreeNode("詳細データ")) {
		ImGui::Checkbox("IsAccel", &isAccel_);
		ImGui::Checkbox("IsDecel", &isDecel_);
		int con = this->consecutiveReceptions_;
		ImGui::InputInt("ConsecutiveRecept", &con);
		float current = currentSpeed_ * GameTimeSystem::GetInstance()->GetDeltaTime();
		ImGui::InputFloat("現在の速度", &currentSpeed_);
		ImGui::InputFloat("現在の速度（dt適応）", &current);
		ImGui::TreePop();
	}

}

void DriveEngine::SpeedCalculation()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	// スピード用のレシオ計算
	const float kMaxRate = 10.0f;	// 最大
	const float kMinRate = 1.0f;	// 最小
	int32_t maxEffective = global->GetIntValue("VehicleEngine", "MaxEffectiveCount");	// エンジンの最大数
	// レート
	float engineCount = (float)owner_->GetStatus()->GetEngine();
	float t = (std::clamp(engineCount, 0.0f, 9.0f) + 1.0f) / (float)maxEffective;
	// 乗算レート
	float plusRate = Ease::Easing(Ease::EaseName::Lerp, kMinRate, kMaxRate, t);
	// エンジンが回転している場合
	if (consecutiveReceptions_ != 0) {
		// 加速度の計算
		const float rideSpeedFactor = GlobalVariables::GetInstance()->GetFloatValue("Player", "RideSpeed");
		// 速度計算
		currentSpeed_ = (float)consecutiveReceptions_ * plusRate * rideSpeedFactor;

		// 全体への影響（速度レートが一定を越えている場合オーバーヒート的な何か）
		OverheatProcess(t);

	}
	// エンジンが回転していない場合
	else {
		// 速度が残っている場合
		if (currentSpeed_ != 0.0f) {
			const float decreValue = 0.05f;
			currentSpeed_ = Ease::Easing(Ease::EaseName::Lerp, currentSpeed_, 0.0f, decreValue);
		}
	}

	// 運転されていなければ
	if (!owner_->IsDrive()) {
		// 速度が残っている場合
		if (currentSpeed_ != 0.0f) {
			const float decreValue = 0.1f;
			currentSpeed_ = Ease::Easing(Ease::EaseName::Lerp, currentSpeed_, 0.0f, decreValue);
		}
	}


	// 切り捨て
	const float discard = 0.75f;
	//currentSpeed_ *= GameTimeSystem::GetInstance()->GetDeltaTime();
	if (std::fabsf(currentSpeed_) <= discard) {
		currentSpeed_ = 0.0f;
	}

}

void DriveEngine::OverheatProcess(const float& SpeedPercentage)
{
	// 押し込みが半分以上かつ
	const float speedLimit = 0.5f;
	const float receptionLimit = sMaxReception / 2.0f;
	if (SpeedPercentage >= speedLimit &&
		std::abs(consecutiveReceptions_) >= receptionLimit) {
		this->owner_->GetStatus()->SetIsOverheat(true);
		GlobalVariables* global = GlobalVariables::GetInstance();
		//float minDPS = 0;
		//float maxDPS = 0;
		float minDPS = global->GetFloatValue("Vehicle", "OverheatMinDamage");
		float maxDPS = global->GetFloatValue("Vehicle", "OverheatMaxDamage");
		// スピード用のレシオ計算
		const float kMaxRate = sMaxRate;	// 最大
		const float kMinRate = sMinRate;	// 最小
		int32_t maxEffective = global->GetIntValue("VehicleEngine", "MaxEffectiveCount");	// エンジンの最大数
		// レート
		float engineCount = (float)owner_->GetStatus()->GetEngine();
		float t = (std::clamp(engineCount, 0.0f, 9.0f) + 1.0f) / (float)maxEffective;
		// 加速度の計算
		const float rideSpeedFactor = GlobalVariables::GetInstance()->GetFloatValue("Player", "RideSpeed");
		// 乗算レート
		float plusRate = Ease::Easing(Ease::EaseName::Lerp, kMinRate, kMaxRate, t);
		float maxRate = receptionLimit * plusRate * rideSpeedFactor;
		t = (std::fabsf(currentSpeed_) - (maxRate)) / maxRate;

		float dps = Ease::Easing(Ease::EaseName::Lerp, minDPS, maxDPS, t);

		// SpeedRate(now / max) = t
		// Ease(minDPS,maxDPS,t)
		owner_->GetStatus()->SetDamagePerSecond(std::fabsf(dps));
	}


}
