#include "GameTimeSystem.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../Tutorial/Tutorial.h"

GameTimeSystem::GameTimeSystem()
{
}

void GameTimeSystem::Initialize()
{
	const float kLimitTime = 120.0f;	// 秒数
	remainingTime_ = kLimitTime;
	isRunning_ = false;
	isPause_ = false;
	SetTimeScale(1.0f);

	tutorialFinished_ = false;
	startAnimationEnds_ = false;
	remainingSeconds_ = 1;

	animationTime_ = 2.0f;

}

void GameTimeSystem::Update()
{

	// チュートリアル
	if (!tutorialFinished_) {
		tutorialFinished_ = Tutorial::GetInstance()->GetIsEnd();
		if (tutorialFinished_) {
			// アニメーションスタート
			remainingTime_ = animationTime_;
		}
		return;
	}

	// スタートしたか
	if (!startAnimationEnds_) {
		float reductionTime = 1.0f * GetDeltaTime();	// 減少時間（1.0 = タイムファクター）
		remainingTime_ -= reductionTime;
		if (remainingTime_ <= 0.0f) {
			startAnimationEnds_ = true;
			GameStart();
		}
		else {
			return;
		}
	}

	// 稼働中
	if (isRunning_ && !isPause_) {
		float reductionTime = 1.0f * GetDeltaTime();	// 減少時間（1.0 = タイムファクター）
		remainingTime_ -= reductionTime;
		remainingSeconds_ = static_cast<int>(remainingTime_);

		// 時間切れ
		if (remainingTime_ <= 0.0f) {
			isRunning_ = false;
			isEnd_ = true;
		}
	}
}

void GameTimeSystem::GameStart()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	const float kLimitSecond = global->GetFloatValue("GameSystem","LimitSecond");
	Start(kLimitSecond);
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
	if (ImGui::Button("ゲーム開始")) {
		GameStart();
	}
	if (ImGui::Button("チェックポイント通過")) {
		CheckpointTimeAffect();
	}
	if (ImGui::Button("一時停止")) {
		Pause();
	}
	if (ImGui::Button("再開")) {
		Resume();
	}
	ImGui::DragFloat("タイムスケール", &timeScale_, 0.01f);
}

void GameTimeSystem::CheckpointTimeAffect()
{
	// チェックポイント通過時のやつ
	GlobalVariables* global = GlobalVariables::GetInstance();
	const float kCheckpointAddSecond = global->GetFloatValue("GameSystem", "CheckpointAddSecond");
	remainingTime_ += kCheckpointAddSecond;
}
