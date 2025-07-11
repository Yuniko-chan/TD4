#include "GameTimeSystem.h"
#include "../../../Engine/Math/DeltaTime.h"

GameTimeSystem::GameTimeSystem()
{
}

void GameTimeSystem::Initialize()
{
	elapsedTime_ = 0.0f;
	isRunning_ = false;
	isPause_ = false;
}

void GameTimeSystem::Update()
{
	// 稼働中
	if (isRunning_ && !isPause_) {
		elapsedTime_ += kDeltaTime_;
	}



}

void GameTimeSystem::Start()
{
	// 初期化
	elapsedTime_ = 0.0f;
	isRunning_ = true;
	isPause_ = false;
}

void GameTimeSystem::Pause()
{
	isPause_ = true;
}

void GameTimeSystem::Resume()
{
	//isRunning_ = true;
	isPause_ = false;
}
