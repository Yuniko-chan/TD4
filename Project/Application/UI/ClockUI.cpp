#include "ClockUI.h"
#include "../../Engine/Math/Ease.h"

void ClockUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
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

void ClockUI::Update()
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
		const float kTimeMax = 2.0f;
		const float kT = (kTimeMax - timerSystem_->GetRemainingTime()) / 2.0f;
		const Vector2 kPosition = { sprite_->GetPosition().x, Ease::Easing(Ease::EaseName::Lerp, 800.0f, 60.0f, kT) };
		sprite_->SetPosition(kPosition);
	}

}
