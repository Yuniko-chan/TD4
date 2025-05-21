#include "VehicleHandling.h"

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
	if (inputX < 0) {
		isLeft_ = true;
	}
}

void VehicleHandling::Update()
{
	// 入力による角度の設定
	if (isRight_) {
		++rightInputCounter_;
	}
	else {
		--rightInputCounter_;
	}
	if (isLeft_) {
		++leftInputCounter_;
	}
	else {
		--leftInputCounter_;
	}
}
