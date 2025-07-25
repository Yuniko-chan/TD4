#include "DriveHandling.h"
#include "../VehicleSystems.h"
#include "../../VehicleCore.h"
#include "../../CarLists.h"

void DriveHandling::HandleInput(const float inputX)
{
	// 前の入力保存
	isRight_.first = isRight_.second;
	isLeft_.first = isLeft_.second;
	// 初期化
	isRight_.second = false;
	isLeft_.second = false;

	// 切り捨て閾値
	const float kDiscardThreshold = 0.25f;

	// 閾値以下なら早期
	if (std::fabsf(inputX) < kDiscardThreshold) {
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
		++inputCounter_;
	}
	// 間隔
	const int duration = 5;	// 間隔
	const int kDecrementDuration = 2;	// 減少間隔
	const int kSteerReturnSensitivity = 3;	// ハンドル戻し感度
	const int kSpDecrementThreshold = 6;	// 減少量を増やすしきい
	const int kSpReturnSensitivity = 5;	// 減少量を増やす感度
	const int kMaxCount = 45;	// 押し込み最大	

	// 入力増加
	if (IsInput() && inputCounter_ % duration == 0) {
		// 左
		if (isLeft_.second) {
			// 特殊処理
			if (consecutiveReceptions_ > kSpDecrementThreshold) {
				consecutiveReceptions_ -= kSpReturnSensitivity;
			}
			// 切り返しの通常処理
			else if (consecutiveReceptions_ > 0) {
				consecutiveReceptions_ -= kSteerReturnSensitivity;
			}
			// 通常処理
			else {
				consecutiveReceptions_--;
			}
		}
		// 右
		else if (isRight_.second) {
			// 特殊処理
			if (consecutiveReceptions_ < -kSpDecrementThreshold) {
				consecutiveReceptions_ += kSpReturnSensitivity;
			}
			// 切り返しの通常処理
			else if (consecutiveReceptions_ < 0) {
				consecutiveReceptions_ += kSteerReturnSensitivity;
			}
			// 通常処理
			else {
				consecutiveReceptions_++;
			}
		}
		inputCounter_ = 0;

	}
	// 非入力での減少処理
	else if (IsNoneInput() && inputCounter_ % kDecrementDuration == 0) {
		int16_t decreValue = 1;
		// 減少量の変化
		float lim = (float)kMaxCount / 3.0f;
		if (std::fabsf((float)consecutiveReceptions_) > (lim)) {
			decreValue = 4;
		}
		else if (std::fabsf((float)consecutiveReceptions_) > ((float)kMaxCount / 5.0f)) {
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
	consecutiveReceptions_ = (int16_t)std::clamp((int)consecutiveReceptions_, -kMaxCount, kMaxCount);
	// 前フレーム
	preSteerDirection_ = steerDirection_;
}

void DriveHandling::PostUpdate(const Vector3& velocity, VehicleStatus* status)
{
	// 速度に応じたハンドルの処理
	const int kMaxCount = 45;	// 押し込み最大	
	float t = (float)std::abs((int)consecutiveReceptions_) / kMaxCount;
	// 最大角度（-1~1,0,1):(-0.5|0.5,0,0.5)
	const float kMaxXDirect = 2.0f;
	const float kMinXDirect = 1.0f;
	const float kMinPropulsion = 5.0f;
	const float kMaxPropulsion = 20.0f;
	
	// 推進力計算
	float propulsion = std::clamp(velocity.z, kMinPropulsion, kMaxPropulsion);
	float propulsionT = (propulsion - kMinPropulsion) / (kMaxPropulsion - kMinPropulsion);

	float limitDirect = Ease::Easing(Ease::EaseName::Lerp, kMinXDirect, kMaxXDirect, propulsionT);
	// プラス方向（右
	if (consecutiveReceptions_ > 0) {
		yaw_ = 0.01f;
		steerDirection_.x = Ease::Easing(Ease::EaseName::Lerp, steerDirection_.x, limitDirect, t);
	}
	// マイナス方向（左
	else if (consecutiveReceptions_ < 0) {
		yaw_ = -0.01f;
		steerDirection_.x = Ease::Easing(Ease::EaseName::Lerp, steerDirection_.x, -limitDirect, t);
	}
	else {
		yaw_ = 0.0f;
		steerDirection_.x = 0.0f;
	}
	// Z設定
	steerDirection_.z = 10.0f;
	steerDirection_.y = 0;

	// 正規化
	steerDirection_ = Vector3::Normalize(steerDirection_);


	// 向きの設定
	Vector3 vehicleDirectionXZ = vehicleDirection_;
	vehicleDirectionXZ.y = 0;
	vehicleDirectionXZ = Vector3::Normalize(vehicleDirectionXZ);
	Matrix4x4 vehicleRotate = Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), vehicleDirectionXZ);
	executeDirection_ = Matrix4x4::TransformNormal(steerDirection_, vehicleRotate);

	Matrix4x4 a = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, steerDirection_);

	//Matrix4x4 b = Matrix4x4::Multiply(a, owner_->GetWorldTransformAdress()->worldMatrix_);

	//Vector3 c = Matrix4x4::TransformNormal(steerDirection_, owner_->GetWorldTransformAdress()->worldMatrix_);

	owner_->posture_ = a * owner_->posture_;
	owner_->GetWorldTransformAdress()->direction_ = Matrix4x4::TransformNormal(steerDirection_, owner_->posture_);

	Matrix4x4 d =  Matrix4x4::DirectionToDirection(Matrix4x4::TransformNormal(Vector3{ 0.0f,0.0f,1.0f }, owner_->GetWorldTransformAdress()->worldMatrix_), Matrix4x4::TransformNormal(steerDirection_, owner_->GetWorldTransformAdress()->worldMatrix_));
	//owner_->GetWorldTransformAdress()->direction_ += Matrix4x4::TransformNormal(owner_->GetWorldTransformAdress()->direction_,d);
	//Vector3 localDirection =
	//if (c.z <0) {
		//c.x *= -c.x;
		//c.z *= -c.z;
		//c.y = std::abs(c.y);
	//}
	//owner_->GetWorldTransformAdress()->direction_ += c;
	//owner_->GetWorldTransformAdress()->direction_ = Vector3::Normalize(owner_->GetWorldTransformAdress()->direction_);
	//owner_->GetWorldTransformAdress()->direction_ = Matrix4x4::TransformNormal(owner_->GetWorldTransformAdress()->direction_,d);

	if (executeDirection_ == Vector3(0.0f, 0.0f, 0.0f)) {
		executeDirection_ = Vector3(0.0f, 0.0f, 1.0f);
	}

	if (executeDirection_ != Vector3(0, 0, 0)) {
		owner_->GetWorldTransformAdress()->direction_ = executeDirection_;
	}
	// タイヤの数、左右
	int rightWheel = status->GetRightWheel();
	int leftWheel = status->GetLeftWheel();
	int tireCount = status->GetTire();
	const int kMax = 5;
	// 入力があれば向きの調整処理
	if (IsInput() && false) {

		// 右
		if (isRight_.second && rightWheel > 0) {
			int value = std::min(rightWheel, kMax);
			float ratio = Ease::Easing(Ease::EaseName::Lerp, 0.75f, 1.25f, (float)value / kMax);
			executeDirection_.x *= ratio;
		}
		// 左
		else if (isLeft_.second && leftWheel > 0) {
			int value = std::min(leftWheel, kMax);
			float ratio = Ease::Easing(Ease::EaseName::Lerp, 0.75f, 1.25f, (float)value / kMax);
			executeDirection_.x *= ratio;
		}
		else if (leftWheel == 0 && rightWheel == 0 && tireCount > 0) {

		}

		else if (leftWheel == 0 && rightWheel == 0 && (std::fabsf(executeDirection_.x) != 0.0f)) {
			executeDirection_.x *= (1.0f / 30.0f);
		}

		executeDirection_ = Vector3::Normalize(executeDirection_);
		const float kRate = 0.01f;
		owner_->GetWorldTransformAdress()->direction_ = Ease::Easing(Ease::EaseName::Lerp, vehicleDirection_, executeDirection_, kRate);
		//float radian = TransformHelper::CalculateXZVectorToRotateRadian(owner_->GetWorldTransformAdress()->direction_, executeDirection_);
		//radian /= 60.0f;
		//owner_->GetWorldTransformAdress()->direction_ = TransformHelper::XZRotateDirection(owner_->GetWorldTransformAdress()->direction_, radian);	
	}
}

void DriveHandling::ImGuiDraw()
{
	ImGui::DragFloat3("Steer", &steerDirection_.x);
}
