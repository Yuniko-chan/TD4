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
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "VehicleEngine";
	// フレームカウント（時間を基に）
	float interval = global->GetFloatValue(groupName, "InputInterval");
	float timming = interval * kDeltaTime_;
	interval = global->GetFloatValue(groupName, "InputDecrementInterval");
	float decrementTimming = (interval) * kDeltaTime_;
	int maxReception = (int)global->GetIntValue(groupName, "InputMaxCount");

	// アクセルキーか受付連続値があれば
	if ((isAccel_ || isDecel_) || consecutiveReceptions_ != 0) {
		inputCounter_ += kDeltaTime_;
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
		float current = accumulatedAccel_ * GameTimeSystem::GetInstance()->GetDeltaTime();
		ImGui::InputFloat("現在の速度", &accumulatedAccel_);
		ImGui::InputFloat("現在の速度（dt適応）", &current);
		ImGui::TreePop();
	}

}

void DriveEngine::SpeedCalculation()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	// スピード用のレシオ計算
	const char* groupName = "VehicleEngine";
	float maxEngineCountAccelFactor = global->GetFloatValue(groupName, "MaxEngineCountAccelFactor");	// 最大
	float minEngineCountAccelFactor = global->GetFloatValue(groupName, "MinEngineCountAccelFactor");	// 最小
	int32_t maxEffective = global->GetIntValue(groupName, "MaxEffectiveCount");	// エンジンの最大数
	float rideSpeedFactor = GlobalVariables::GetInstance()->GetFloatValue(groupName, "AccelerationMultiplier");
	// レート
	float engineCount = (float)owner_->GetStatus()->GetEngine();
	engineCount = std::clamp(engineCount, 1.0f, (float)maxEffective);
	float t = engineCount / (float)maxEffective;
	// 乗算レート
	float engineCountEffectiveFactor = Ease::Easing(Ease::EaseName::Lerp, minEngineCountAccelFactor, maxEngineCountAccelFactor, t);
	
	// 加速処理
	if (consecutiveReceptions_ != 0) {
		// 速度計算
		accumulatedAccel_ = (float)consecutiveReceptions_ * engineCountEffectiveFactor * rideSpeedFactor;

		// 全体への影響（速度レートが一定を越えている場合オーバーヒート的な何か）
		OverheatProcess(t);

	}

	// 減速処理
	if (accumulatedAccel_ != 0.0f) {
		float decelerationFactor = 0.0f;
		if (owner_->IsDrive() && consecutiveReceptions_ == 0) {
			decelerationFactor = global->GetFloatValue(groupName, "IdleDecelerationFactor");
			accumulatedAccel_ = Ease::Easing(Ease::EaseName::Lerp, accumulatedAccel_, 0.0f, decelerationFactor);
		}
		else if (!owner_->IsDrive()){
			decelerationFactor = global->GetFloatValue(groupName, "StopDecelerationFactor");
			accumulatedAccel_ = Ease::Easing(Ease::EaseName::Lerp, accumulatedAccel_, 0.0f, decelerationFactor);
		}

	}

	// 切り捨て
	const float discard = 0.75f;
	//currentSpeed_ *= GameTimeSystem::GetInstance()->GetDeltaTime();
	if (std::fabsf(accumulatedAccel_) <= discard) {
		accumulatedAccel_ = 0.0f;
	}

}

void DriveEngine::OverheatProcess(const float& SpeedPercentage)
{
	SpeedPercentage;
	// 押し込みが半分以上かつ
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "VehicleEngine";
	int heatEngineCount = (int)global->GetIntValue(groupName, "OverheatEngineCountThreshold");	// 過熱する閾値
	int maxReception = (int)global->GetIntValue(groupName, "InputMaxCount");	// 受付最大
	float engineCount = (float)owner_->GetStatus()->GetEngine();	// 現在のエンジン数
	int32_t maxEffective = global->GetIntValue(groupName, "MaxEffectiveCount");	// エンジンの最大数
	engineCount = std::clamp(engineCount, 1.0f, (float)maxEffective);

	if ((int)engineCount >= heatEngineCount &&
		std::abs(consecutiveReceptions_) >= (maxReception / 2)) {
		this->owner_->GetStatus()->SetIsOverheat(true);
		float minDPS = global->GetFloatValue("Vehicle", "OverheatMinDamage");
		float maxDPS = global->GetFloatValue("Vehicle", "OverheatMaxDamage");
		// スピード用のレシオ計算
		float maxEngineCountAccelFactor = global->GetFloatValue(groupName, "MaxEngineCountAccelFactor");	// 最大
		float minEngineCountAccelFactor = global->GetFloatValue(groupName, "MinEngineCountAccelFactor");	// 最小
		float rideSpeedFactor = GlobalVariables::GetInstance()->GetFloatValue(groupName, "AccelerationMultiplier");
		// レート
		float t = engineCount / (float)maxEffective;
		// 乗算レート
		float engineCountEffectiveFactor = Ease::Easing(Ease::EaseName::Lerp, minEngineCountAccelFactor, maxEngineCountAccelFactor, t);
		// 加速度の最大値
		float accelLimit = maxReception * engineCountEffectiveFactor * rideSpeedFactor;
		t = std::fabsf(accumulatedAccel_) / accelLimit;

		// 加速度の割合からダメージを計算
		float dps = Ease::Easing(Ease::EaseName::Lerp, minDPS, maxDPS, t);
		owner_->GetStatus()->SetDamagePerSecond(std::fabsf(dps));
	}


}
