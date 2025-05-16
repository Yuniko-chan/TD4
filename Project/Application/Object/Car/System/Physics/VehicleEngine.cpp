#include "VehicleEngine.h"
#include "../../../Engine/Math/Ease.h"

void VehicleEngine::Update()
{
	// フレームカウント
	const int timming = 10;
	const int maxReception = 30;

	// アクセルキーか受付連続値があれば
	if (isAccel_ || consecutiveReceptions_ > 0) {
		accelInputCounter_++;

		// 加速量上昇
		if (accelInputCounter_ % timming == 0 && isAccel_) {
			consecutiveReceptions_++;
		}
		// 加速量低下
		else if(accelInputCounter_ % timming == 0 && !isAccel_){
			consecutiveReceptions_--;
		}

		// 最大値に
		if (consecutiveReceptions_ > maxReception) {
			consecutiveReceptions_ = maxReception;
		}
	}

	// スピード用のレシオ計算
	const float minusRate = 2.0f / 5.0f;
	// エンジンが回転している場合
	if (consecutiveReceptions_ != 0) {
		speedRatio_ = (float)consecutiveReceptions_ * (minusRate);
	}
	// エンジンが回転していない場合
	else {
		// 速度が残っている場合
		if (speedRatio_ != 0.0f) {
			const float decreValue = 0.01f;
			speedRatio_ = Ease::Easing(Ease::EaseName::Lerp, speedRatio_, 0.0f, decreValue);
		}
	}

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
}
