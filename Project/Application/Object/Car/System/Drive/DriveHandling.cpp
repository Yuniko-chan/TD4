#include "DriveHandling.h"
#include "../VehicleSystems.h"
#include "../../VehicleCore.h"
#include "../../CarLists.h"
#include "../../../GameTimer/GameTimeSystem.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void DriveHandling::HandleInput(const float inputX)
{
	// 前の入力保存
	isRight_.first = isRight_.second;
	isLeft_.first = isLeft_.second;
	// 初期化
	isRight_.second = false;
	isLeft_.second = false;

	// 閾値以下なら早期
	if (std::fabsf(inputX) < kInputDiscardThreshold) {
		return;
	}

	// 右
	if (inputX > 0) {
		isRight_.second = true;
	}
	// 左
	else if (inputX < 0) {
		isLeft_.second = true;
	}
}

void DriveHandling::PreUpdate()
{
	// カウント
	if (IsInput() || consecutiveReceptions_ != 0) {
		inputCounter_ += GameTimeSystem::GetInstance()->GetDeltaTime();
	}
	
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "VehicleHandling";
	// 間隔
	float interval = kDeltaTime_ * global->GetFloatValue(groupName, "InputInterval");	// 間隔
	float decrementInterval = kDeltaTime_ * global->GetFloatValue(groupName, "InputDecrementInterval");
	int steerReturnSensitivity = kSteerReturnSensitivity;	// 切り返しの感度
	int steerReturnAccelThreshold = kSpDecrementThreshold;	// より大きいかを判断する閾値
	int highInputReturnSensitivity = kSpReturnSensitivity;	// 切り返しの際により大きいときの感度
	int maxCount = kMaxPressCount;	// 押し込み最大	

	// 入力増加
	if (IsInput() && inputCounter_ >= interval) {
		// 左
		if (isLeft_.second) {
			// 特殊処理
			if (consecutiveReceptions_ > steerReturnAccelThreshold) {
				consecutiveReceptions_ -= highInputReturnSensitivity;
			}
			// 切り返しの通常処理
			else if (consecutiveReceptions_ > 0) {
				consecutiveReceptions_ -= steerReturnSensitivity;
			}
			// 通常処理
			else {
				consecutiveReceptions_--;
			}
		}
		// 右
		else if (isRight_.second) {
			// 特殊処理
			if (consecutiveReceptions_ < -steerReturnAccelThreshold) {
				consecutiveReceptions_ += highInputReturnSensitivity;
			}
			// 切り返しの通常処理
			else if (consecutiveReceptions_ < 0) {
				consecutiveReceptions_ += steerReturnSensitivity;
			}
			// 通常処理
			else {
				consecutiveReceptions_++;
			}
		}
		inputCounter_ = 0;

	}
	// 非入力での減少処理
	else if (IsNoneInput() && inputCounter_ >= decrementInterval) {
		int16_t decreValue = 1;
		// 減少量の変化
		float lim = (float)maxCount / 3.0f;
		if (std::fabsf((float)consecutiveReceptions_) > (lim)) {
			decreValue = 4;
		}
		else if (std::fabsf((float)consecutiveReceptions_) > ((float)maxCount / 5.0f)) {
			decreValue = 2;
		}

		// 受付の変化処理
		if (consecutiveReceptions_ > 0) {
			consecutiveReceptions_ -= decreValue;
		}
		else if (consecutiveReceptions_ < 0) {
			consecutiveReceptions_ += decreValue;
		}
		inputCounter_ = 0;
	}

	// カウントを最大値内に制限
	consecutiveReceptions_ = (int16_t)std::clamp((int)consecutiveReceptions_, -maxCount, maxCount);
	// 前フレーム
	preTireDirection_ = tireDirection_;
}

void DriveHandling::PostUpdate(const Vector3& velocity, VehicleStatus* status)
{
	// ハンドルのデータ処理
	GetUIHandlingData((float)status->GetLeftWheel(), (float)status->GetRightWheel());

	// リセット処理
	// if（プレイヤーが操作している状態でないなら
	if (!owner_->IsPlayer() && !onReset_) {
		onReset_ = std::bind(&DriveHandling::Reset, this);
		onReset_.value()();
		return;
	}
	else if (onReset_) {
		onReset_ = std::nullopt;
	}

	// グローバル
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "VehicleHandling";
	static bool isDebug = false;

	// 速度に応じたハンドルの処理
	float t = (float)std::abs((int)consecutiveReceptions_) / kMaxPressCount;
	// 最大角度（-1~1,0,1):(-0.5|0.5,0,0.5)
	float maxAngle = global->GetFloatValue(groupName, "SteerMaxAngle");
	float minAngle = global->GetFloatValue(groupName, "SteerMinAngle");
	// 制限の向き
	float limitAngle = Ease::Easing(Ease::EaseName::Lerp, minAngle, maxAngle, t);

	if (consecutiveReceptions_ < 0) {
		limitAngle *= -1.0f;
	}
	//steerDirection_.x = Ease::Easing(Ease::EaseName::Lerp, steerDirection_.x, limitAngle, 0.1f);

	// Z設定
	steerDirection_.z = 10.0f;
	steerDirection_.y = 0;

	// ステータス（タイヤの数など）を適応
	steerDirection_ = ApplyStatusToHandling(status, limitAngle);

	if (consecutiveReceptions_ == 0) {
		steerDirection_.x = 0.0f;
	}

	// 正規化前にタイヤ向きに適応
	ApplyHandlingToTire();

	// 低速時の減衰値
	float lowSpeedSteerAttenuation = kLowSpeedSteerAttenuation;	// 最低減衰値（n ~ 1.0f)
	float speedSteerAttenuation = 0.0f;	// 速度に応じたステア減衰値
	float lowSpeedLimit = kLowSpeedLimit;	// 減衰が掛かる最大値


	// 遅すぎるときの減少処理
	if (std::fabsf(velocity.z) <= lowSpeedLimit) {
		float attenuationT = std::fabsf(velocity.z) / lowSpeedLimit;
		attenuationT = std::clamp(attenuationT, 0.0f, 1.0f);
		speedSteerAttenuation = Ease::Easing(Ease::EaseName::Lerp, lowSpeedSteerAttenuation, 1.0f, attenuationT);
		steerDirection_.x *= speedSteerAttenuation;
	}

	// 正規化
	steerDirection_ = Vector3::Normalize(steerDirection_);


	// 向きの設定
	//Vector3 vehicleDirectionXZ = vehicleDirection_;
	//vehicleDirectionXZ.y = 0;
	//vehicleDirectionXZ = Vector3::Normalize(vehicleDirectionXZ);
	//Matrix4x4 vehicleRotate = Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), vehicleDirectionXZ);
	//executeDirection_ = Matrix4x4::TransformNormal(steerDirection_, vehicleRotate);

	//左右回転のdirectionから回転行列生成
	Matrix4x4 fromDirectionRotateMatrix = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, steerDirection_);

	owner_->posture_ = fromDirectionRotateMatrix * owner_->posture_;
	owner_->GetWorldTransformAdress()->direction_ = Matrix4x4::TransformNormal(steerDirection_, owner_->posture_);

	//if (executeDirection_ == Vector3(0.0f, 0.0f, 0.0f)) {
	//	executeDirection_ = Vector3(0.0f, 0.0f, 1.0f);
	//}

	//if (executeDirection_ != Vector3(0, 0, 0)) {
	//	owner_->GetWorldTransformAdress()->direction_ = executeDirection_;
	//}
}

void DriveHandling::Reset()
{
	// 向き
	steerDirection_ = Vector3(0, 0, 1);
	// カウント
	consecutiveReceptions_ = 0;
	inputCounter_ = 0;
	// フラグ
	isLeft_ = {};
	isRight_ = {};
}

Vector3 DriveHandling::ApplyStatusToHandling(VehicleStatus* status , const float& angle)
{
	GlobalVariables* global = GlobalVariables::GetInstance();

	// 車体の向き
	Vector3 vehicleDirection = owner_->GetWorldTransformAdress()->direction_;
	// 結果
	Vector3 result = steerDirection_;
	// 入力があれば向きの調整処理
	int rightWheel = status->GetRightWheel();	// 右タイヤ
	int leftWheel = status->GetLeftWheel();	// 左タイヤ
	//int tireCount = status->GetTire();	// タイヤの総数
	const int kMaxEffectiveTire = global->GetIntValue("VehicleHandling", "MaxEffectiveCount");
	// 入力があるか
	int applyValue = 0;
	// 右にタイヤあるときの右ハンドル
	if (isRight_.second && rightWheel > 0) { applyValue = std::min(rightWheel, kMaxEffectiveTire); }
	// 左にタイヤあるときの左ハンドル
	else if (isLeft_.second && leftWheel > 0) { applyValue = std::min(leftWheel, kMaxEffectiveTire); }

	Vector3 lim = Vector3::Normalize(Vector3(angle, 0.0f, 10.0f));

	float tireT = (float)applyValue / (float)kMaxEffectiveTire;
	float minLerp = global->GetFloatValue("VehicleHandling", "TireLerpMin");
	float maxLerp = global->GetFloatValue("VehicleHandling", "TireLerpMax");
	float handleT = Ease::Easing(Ease::EaseName::Lerp, minLerp, maxLerp, tireT);
	result.x = Ease::Easing(Ease::EaseName::Lerp, steerDirection_.x, lim.x, handleT);

	//float decre = Ease::Easing(Ease::EaseName::Lerp, (1.0f / 10.0f), (1.0f / 1.0f), tireT);
	//result.x *= (decre);

	// 左右にタイヤがなければ減少させる
	if ((leftWheel == 0 && rightWheel == 0) && (std::fabsf(result.x) != 0.0f)) {
		const float kMinimalFactor = (1.0f / 30.0f);
		result.x *= kMinimalFactor;
	}

	return Vector3(result);
}

void DriveHandling::ApplyHandlingToTire()
{
	tireDirection_ = steerDirection_;
	float tireDirectionFactor = 1.85f;
	tireDirection_.x *= tireDirectionFactor;
	tireDirection_.z = 1.0f;
	tireDirection_ = Vector3::Normalize(tireDirection_);
}

void DriveHandling::GetUIHandlingData(const float& leftTire, const float& rightTire)
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	const int kMaxEffectiveTire = global->GetIntValue("VehicleHandling", "MaxEffectiveCount");

	// タイヤごとの現在の状態
	currentLeftTire_ = std::clamp(float(leftTire / kMaxEffectiveTire), 0.0f, 1.0f);
	currentRightTire_ = std::clamp(float(rightTire / kMaxEffectiveTire), 0.0f, 1.0f);
}

void DriveHandling::ImGuiDraw()
{
	ImGui::SeparatorText("ドライブハンドル");

	ImGui::DragFloat("左タイヤの割合", &currentLeftTire_);
	ImGui::DragFloat("右タイヤの割合", &currentRightTire_);

	ImGui::DragFloat3("Steer", &steerDirection_.x);
}
