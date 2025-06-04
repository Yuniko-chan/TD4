#include "VehicleHandling.h"
#include "VehicleSystems.h"
#include "../VehicleCore.h"
#include "../../Utility/Calc/TransformHelper.h"

#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/2D/ImguiManager.h"

void VehicleHandling::HandleInput(const float inputX)
{
	// 前の入力保存
	isRight_.first = isRight_.second;
	isLeft_.first = isLeft_.second;
	// 初期化
	isRight_.second = false;
	isLeft_.second = false;

	// 右
	if (inputX > 0) {
		isRight_.second = true;
	}
	// 左
	else if (inputX < 0) {
		isLeft_.second = true;
	}
}

void VehicleHandling::PreUpdate()
{
	// カウント
	if (IsInput() || consecutiveReceptions_ != 0) {
		++inputCounter_;
	}
	// 間隔
	const int duration = 6;	// 間隔
	const int spDecrement = 6;	// 減少の量を増やすしきい
	const int kMaxCount = 45;	// 押し込み最大	

	// 入力増加
	if (inputCounter_ % duration == 0) {
		if (isLeft_.second) {
			// 特殊処理
			if (consecutiveReceptions_ > spDecrement) {
				consecutiveReceptions_ -= spDecrement;
			}
			else {
				consecutiveReceptions_--;
			}
		}
		else if (isRight_.second) {
			// 特殊処理
			if (consecutiveReceptions_ < -spDecrement) {
				consecutiveReceptions_ += spDecrement;
			}
			else {
				consecutiveReceptions_++;
			}
		}
		inputCounter_ = 0;
	}
	// 非入力での減少処理
	else if (!IsInput() && (inputCounter_ % (duration / 2) == 0)) {
		if (consecutiveReceptions_ > 0) {
			consecutiveReceptions_ -= (kMaxCount / 5);
			if (consecutiveReceptions_ < 0) {
				consecutiveReceptions_ = 0;
			}
		}
		else if (consecutiveReceptions_ < 0) {
			consecutiveReceptions_ += (kMaxCount / 5);
			if (consecutiveReceptions_ > 0) {
				consecutiveReceptions_ = 0;
			}
		}
		inputCounter_ = 0;
	}

	// カウントを最大値内に制限
	consecutiveReceptions_ = (int16_t)std::clamp((int)consecutiveReceptions_, -kMaxCount, kMaxCount);

	float t = (float)std::abs((int)consecutiveReceptions_) / kMaxCount;
	const float limitDirect = 2.0f;	// 最大角度（-1~1,0,1):(-0.5|0.5,0,0.5)
	// プラス方向（右
	if (consecutiveReceptions_ > 0) {
		steerDirection_.x = Ease::Easing(Ease::EaseName::Lerp, steerDirection_.x, limitDirect, t);
	}
	// マイナス方向（左
	else if (consecutiveReceptions_ < 0) {
		steerDirection_.x = Ease::Easing(Ease::EaseName::Lerp, steerDirection_.x, -limitDirect, t);
	}
	else {
		steerDirection_.x = 0.0f;
	}
	steerDirection_.z = 1.0f;

	// 正規化
	steerDirection_ = Vector3::Normalize(steerDirection_);

	Matrix4x4 vehicleRotate = Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), vehicleDirection_);
	executeDirection_ = Matrix4x4::TransformNormal(steerDirection_, vehicleRotate);

	//float radian = TransformHelper::CalculateXZVectorToRotateRadian(owner_->GetWorldTransformAdress()->direction_, executeDirection_);
	//radian /= 60.0f;
	//executeDirection_ = TransformHelper::XZRotateDirection(executeDirection_, radian);

	if (executeDirection_ == Vector3(0.0f, 0.0f, 0.0f)) {
		executeDirection_ = Vector3(0.0f, 0.0f, 1.0f);
	}

}

void VehicleHandling::PostUpdate(const Vector3& velocity, VehicleStatus* status)
{
	// 速度が無く動いていなかったら
	float length = Vector3::Length(velocity);
	const float threshold = 0.001f;
	// 閾値より速度ベクトルの大きさがなければ早期
	if (std::fabsf(length) <= threshold) {
		return;
	}

	// タイヤの数、左右
	int rightWheel = status->GetRightWheel();
	int leftWheel = status->GetLeftWheel();
	int tireCount = status->GetTire();
	const int kMax = 5;
	// 入力があれば向きの調整処理
	if (IsInput()) {

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

		//owner_->GetWorldTransformAdress()->direction_ = executeDirection_;

		float radian = TransformHelper::CalculateXZVectorToRotateRadian(owner_->GetWorldTransformAdress()->direction_, executeDirection_);

		radian /= 60.0f;

		owner_->GetWorldTransformAdress()->direction_ = TransformHelper::XZRotateDirection(owner_->GetWorldTransformAdress()->direction_, radian);
	}
}

void VehicleHandling::ImGuiDraw()
{
	ImGui::DragFloat3("Steer", &steerDirection_.x);
}
