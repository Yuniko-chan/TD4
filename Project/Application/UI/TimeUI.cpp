#include "TimeUI.h"
#include "../../Engine/Math/Ease.h"

void TimeUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
{

	BaseUI::Initialize(textureHandle, position, size);

	textureSize_ = Vector2{ 512.0f,512.0f };
	sprite_->SetTextureSize(textureSize_);
	sprite_->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });

	timerSystem_ = GameTimeSystem::GetInstance();
	// 位置固定
	const Vector2 kPosition = { sprite_->GetPosition().x, 800.0f };
	sprite_->SetPosition(kPosition);

}

void TimeUI::Update()
{
	// 移動更新
	MoveUpdate();

	// 時間更新
	TimerUpdate();

}

void TimeUI::ImGuiDraw()
{

	BaseUI::ImGuiDraw();

}

void TimeUI::MoveUpdate()
{

	bool startAnimationEnds = timerSystem_->GetStartAnimationEnds();
	bool tutorialFinished = timerSystem_->GetTutorialFinished();

	if (startAnimationEnds && tutorialFinished) {
		// 位置固定
		const Vector2 kPosition = { sprite_->GetPosition().x, 60.0f };
		sprite_->SetPosition(kPosition);
		return;
	}

	if (!startAnimationEnds && tutorialFinished) {
		// 移動アニメーション
		const float kTimeMax = timerSystem_->GetAnimationTime();
		const float kT = (kTimeMax - timerSystem_->GetRemainingTime()) / kTimeMax;
		const Vector2 kPosition = { sprite_->GetPosition().x, Ease::Easing(Ease::EaseName::Lerp, 800.0f, 60.0f, kT) };
		sprite_->SetPosition(kPosition);
	}

}

void TimeUI::TimerUpdate()
{

	int remainingTime = timerSystem_->GetRemainingSeconds();

	int leftTopNumX = 0;

	const float kSizeX = 512.0f;

	switch (digitIndex_)
	{
	case TimeUI::kDigitIndexOneHundred:
		leftTopNumX = remainingTime / 100;
		break;
	case TimeUI::kDigitIndexTen:
		leftTopNumX = remainingTime % 100 / 10;
		break;
	case TimeUI::kDigitIndexOne:
		leftTopNumX = remainingTime % 10;
		break;
	case TimeUI::kDigitIndexOfCount:
	default:
		assert(0);
		break;
	}

	// 左上
	textureLeftTop_.x = leftTopNumX * kSizeX;
	sprite_->SetTextureLeftTop(textureLeftTop_);

}
