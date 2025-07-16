#include "GameTimeSystem.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/2D/ImguiManager.h"

GameTimeSystem::GameTimeSystem()
{
}

void GameTimeSystem::Initialize()
{
	const float kLimitTime = 120.0f;	// 秒数
	remainingTime_ = kLimitTime;
	isRunning_ = false;
	isPause_ = false;
}

void GameTimeSystem::Update()
{
	// 稼働中
	if (isRunning_ && !isPause_) {
		float reductionTime = 1.0f * kDeltaTime_;	// 減少時間（1.0 = タイムファクター）
		remainingTime_ -= reductionTime;
		remainingSeconds_ = static_cast<int>(remainingTime_);

		// 時間切れ
		if (remainingTime_ <= 0.0f) {
			isRunning_ = false;
			isEnd_ = true;
		}
	}
}

void GameTimeSystem::Start()
{
	// 初期化
	remainingTime_ = 0.0f;
	isRunning_ = true;
	isPause_ = false;
}

void GameTimeSystem::Start(const float& limitTime)
{
	remainingTime_ = limitTime;
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
	ImGui::InputFloat("残り時間", &remainingTime_);
	ImGui::InputInt("残り秒数", &remainingSeconds_);
	static float kLimit = 1.0f;
	ImGui::InputFloat("制限時間（秒数）", &kLimit);
	if (ImGui::Button("開始")) {
		Start(kLimit);
	}
	if (ImGui::Button("一時停止")) {
		Pause();
	}
	if (ImGui::Button("再開")) {
		Resume();
	}
}
