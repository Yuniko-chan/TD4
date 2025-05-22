#include "VehicleHandling.h"
#include "../VehicleSystems.h"
#include "../../VehicleCore.h"

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

void VehicleHandling::Update()
{
	// カウント
	if (IsInput() || consecutiveReceptions_ != 0) {
		++inputCounter_;
	}
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
	const float limitDirect = 0.75f;
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

void VehicleHandling::ImGuiDraw()
{
	ImGui::DragFloat3("Steer", &steerDirection_.x);
}
