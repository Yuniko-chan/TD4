#include "GameTimeSystem.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/2D/ImguiManager.h"

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
		elapsedSecond_ = static_cast<int>(elapsedTime_);
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

void GameTimeSystem::ImGuiDraw()
{
	ImGui::InputFloat("経過時間", &elapsedTime_);
	ImGui::InputInt("経過秒数", &elapsedSecond_);
	if (ImGui::Button("開始")) {
		Start();
	}
	if (ImGui::Button("一時停止")) {
		Pause();
	}
	if (ImGui::Button("再開")) {
		Resume();
	}
}
