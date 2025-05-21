#include "VehicleHandling.h"
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
		isDirection_ = kRight;
	}
	// 左
	else if (inputX < 0) {
		isLeft_ = true;
		isDirection_ = kLeft;
	}
}

void VehicleHandling::Update()
{
	// カウント
	if (IsInput() || consecutiveReceptions_ != 0) {
		++inputCounter_;
	}
	// 増加
	if (inputCounter_ % 10 == 0) {
		if (isLeft_) {
			consecutiveReceptions_--;
		}
		else if (isRight_) {
			consecutiveReceptions_++;
		}
	}
	// 減少
	else if (!IsInput() && (inputCounter_ % 5 == 0)) {
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
	// プラス方向（右
	if (consecutiveReceptions_ > 0) {
		steerDirection_.x = Ease::Easing(Ease::EaseName::Lerp, 0.0f, 1.0f, t);
	}
	// マイナス方向（左
	else if (consecutiveReceptions_ < 0) {
		steerDirection_.x = Ease::Easing(Ease::EaseName::Lerp, 0.0f, -1.0f, t);
	}
	else {
		steerDirection_.x = 0.0f;
	}
	steerDirection_.z = 1.0f;

	// 正規化
	steerDirection_ = Vector3::Normalize(steerDirection_);
}

void VehicleHandling::ImGuiDraw()
{
	ImGui::DragFloat3("Steer", &steerDirection_.x);
}
