#include "VehicleEngine.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/3D/Transform/WorldTransform.h"

#include "../../../Player/DebugData/PlayerDebugData.h"

void VehicleEngine::Update()
{
	// フレームカウント
	const int timming = 10;
	const int maxReception = 10;

	// アクセルキーか受付連続値があれば
	if (isAccel_ || consecutiveReceptions_ > 0) {
		accelInputCounter_++;

		// 加速量上昇
		if (accelInputCounter_ % timming == 0 && isAccel_) {
			consecutiveReceptions_++;
			accelInputCounter_ = 0;
		}
		// 加速量低下
		else if(accelInputCounter_ % 5 == 0 && !isAccel_){
			consecutiveReceptions_--;
			accelInputCounter_ = 0;
		}

		// 最大値に
		if (consecutiveReceptions_ > maxReception) {
			consecutiveReceptions_ = maxReception;
		}
	}

	// スピード用のレシオ計算
	const float minusRate = 3.0f;
	// エンジンが回転している場合
	if (consecutiveReceptions_ != 0) {
		speedRatio_ = (float)consecutiveReceptions_ * (minusRate);
	}
	// エンジンが回転していない場合
	else {
		// 速度が残っている場合
		if (speedRatio_ == 0.0f) {

		}
		else {
			const float decreValue = 0.05f;
			speedRatio_ = Ease::Easing(Ease::EaseName::Lerp, speedRatio_, 0.0f, decreValue);
		}
	}
	
	// 向きに併せるためのベクトル
	Vector3 newDirection = Matrix4x4::TransformNormal(Vector3(handringDirect_), Matrix4x4::MakeRotateYMatrix(moveDirect_.y));
	// 加速度の計算
	acceleration_ = newDirection * (speedRatio_ * PlayerDebugData::sMoveData.rideSpeed);

	//// ブレーキが踏まれているか
	//if (isDecel_ || consecutiveReceptions_ < 0) {
	//	decelInputCounter_++;

	//	// 減速処理
	//	if (decelInputCounter_ % timming == 0 && isDecel_) {
	//		consecutiveReceptions_--;
	//	}
	//	else if (accelInputCounter_ % timming == 0 && !isDecel_) {
	//		consecutiveReceptions_++;
	//	}
	//}
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
	
	controlDirect_ = keyConfig->GetLeftStick()->x;

	handringDirect_.x = Ease::Easing(Ease::EaseName::Lerp, -1.0f, 1.0f, (controlDirect_ + 1.0f) / 2.0f);
	handringDirect_.z = 1.0f;
	handringDirect_ = Vector3::Normalize(handringDirect_);
}

void VehicleEngine::ImGuiDraw()
{
	ImGui::Checkbox("IsAccel", &isAccel_);
	ImGui::Checkbox("IsDecel", &isDecel_);

	int con = this->accelInputCounter_;
	ImGui::InputInt("AccelInput", &con);
	con = this->decelInputCounter_;
	ImGui::InputInt("DecelInput", &con);
	con = this->consecutiveReceptions_;
	ImGui::InputInt("ConsecutiveRecept", &con);
	ImGui::DragFloat("SpeedRatio", &speedRatio_);
	// ハンドル角
	ImGui::DragFloat("LeftstickX", &controlDirect_);
	ImGui::DragFloat3("Handle", &handringDirect_.x);
}
