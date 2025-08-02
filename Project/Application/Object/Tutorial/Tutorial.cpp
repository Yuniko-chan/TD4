#include "Tutorial.h"
#include "../../ClearSceneObject/CourseTraversalSystem.h"
#include "../Car/VehicleCore.h"

Tutorial::Tutorial()
{
}

void Tutorial::Initialize()
{

	// 終了したか
	isEnd_ = false;

	// エンジン確認
	engineCheck_ = false;

	// タイヤ確認
	tireCheck_ = false;

	// フレーム確認
	frameCheck_ = false;

	// コア
	vehicleCore_ = nullptr;

	// 踏破数
	courseTraversalNum_ = nullptr;

}

void Tutorial::Update()
{

	// 終了したか
	if (isEnd_) {
		return;
	}
	else {
		if (*courseTraversalNum_ > 0) {
			isEnd_ = true;
		}
	}

	if (vehicleCore_->IsDrive()) {
		engineCheck_ = false;
		tireCheck_ = false;
		frameCheck_ = false;
		isDrive_ = true;
		return;
	}
	else {
		isDrive_ = false;
	}

	VehicleStatus* status = vehicleCore_->GetStatus();

	// エンジン確認
	if (status->GetEngine() > 0) {
		engineCheck_ = true;
	}
	else {
		engineCheck_ = false;
	}

	// タイヤ確認
	if (status->GetTire() > 0) {
		tireCheck_ = true;
	}
	else {
		tireCheck_ = false;
	}

	// フレーム確認
	if (status->GetFrame() > 0) {
		frameCheck_ = true;
	}
	else {
		frameCheck_ = false;
	}

}
