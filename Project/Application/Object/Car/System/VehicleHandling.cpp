#include "VehicleHandling.h"
#include "VehicleSystems.h"
#include "../VehicleCore.h"
#include "../../Utility/Calc/TransformHelper.h"

#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/2D/ImguiManager.h"

void VehicleHandling::HandleInput(const float inputX)
{
	// 初期化
	isRight_ = false;
	isLeft_ = false;

	// 右
	if (inputX > 0) {
		isRight_ = true;
	}
	// 左
	else if (inputX < 0) {
		isLeft_ = true;
	}
}

void VehicleHandling::PreUpdate()
{
	// カウント
	if (IsInput() || consecutiveReceptions_ != 0) {
		++inputCounter_;
	}
	// 間隔
	const int duration = 6;

	// 増加
	if (inputCounter_ % duration == 0) {
		if (isLeft_) {
			consecutiveReceptions_--;
		}
		else if (isRight_) {
			consecutiveReceptions_++;
		}
		inputCounter_ = 0;
	}
	// 減少
	else if (!IsInput() && (inputCounter_ % (duration / 2) == 0)) {
		if (consecutiveReceptions_ > 0) {
			consecutiveReceptions_--;
		}
		else if (consecutiveReceptions_ < 0) {
			consecutiveReceptions_++;
		}
		inputCounter_ = 0;
	}

	// カウントを最大値内に制限
	const int kMaxCount = 30;
	consecutiveReceptions_ = (int16_t)std::clamp((int)consecutiveReceptions_, -kMaxCount, kMaxCount);

	float t = (float)std::abs((int)consecutiveReceptions_) / kMaxCount;
	const float limitDirect = 1.0f;	// 最大角度（-1~1,0,1):(-0.5|0.5,0,0.5)
	// プラス方向（右
	if (consecutiveReceptions_ > 0) {
		steerDirection_.x = Ease::Easing(Ease::EaseName::Lerp, 0.0f, limitDirect, t);
	}
	// マイナス方向（左
	else if (consecutiveReceptions_ < 0) {
		steerDirection_.x = Ease::Easing(Ease::EaseName::Lerp, 0.0f, -limitDirect, t);
	}
	else {
		steerDirection_.x = 0.0f;
	}
	steerDirection_.z = 1.0f;

	// 正規化
	steerDirection_ = Vector3::Normalize(steerDirection_);

}

void VehicleHandling::PostUpdate(const Vector3& velocity, float leftWheel, float rightWheel)
{
	// 速度が無く動いていなかったら
	float length = Vector3::Length(velocity);
	const float threshold = 0.001f;
	// 閾値より速度ベクトルの大きさがなければ早期
	if (std::fabsf(length) <= threshold) {
		return;
	}

	// 入力があれば向きの調整処理
	if (IsInput()) {

		// 右
		if (steerDirection_.x > 0) {
			if (rightWheel > 0) {
				steerDirection_.x *= 1.1f;
			}
			//// 右 ー 左＞右
			//if (leftWheel > rightWheel) {
			//	steerDirection_.x *= 0.75f;
			//}
			//// 右 ー 右＞左
			//else if (leftWheel < rightWheel) {
			//	steerDirection_.x = 1.25f;
			//}
			//// 右 ー 一致
			//else {

			//}
		}
		// 左
		else if (steerDirection_.x < 0) {
			if (leftWheel > 0) {
				steerDirection_.x *= 1.1f;
			}
			//// 左 ー 左＞右
			//if (leftWheel > rightWheel) {
			//	steerDirection_.x = 1.25f;
			//}
			//// 左 ー 右＞左
			//else if (leftWheel < rightWheel) {
			//	steerDirection_.x *= 0.75f;
			//}
			//// 左 ー 一致
			//else {

			//}
		}
		//else if(leftWheel == 0 && rightWheel == 0 && ())

		else if (leftWheel == 0 && rightWheel == 0 && (std::fabsf(steerDirection_.x) != 0.0f)) {
			steerDirection_.x *= (1.0f / 30.0f);
		}

		float radian = TransformHelper::CalculateXZVectorToRotateRadian(owner_->GetWorldTransformAdress()->direction_, steerDirection_);

		radian /= 60.0f;

		owner_->GetWorldTransformAdress()->direction_ = TransformHelper::XZRotateDirection(owner_->GetWorldTransformAdress()->direction_, radian);
	}
}

void VehicleHandling::ImGuiDraw()
{
	ImGui::DragFloat3("Steer", &steerDirection_.x);
}
