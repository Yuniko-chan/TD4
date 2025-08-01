#include "TimeUI.h"
#include "../Object/GameTimer/GameTimeSystem.h"

void TimeUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
{

	BaseUI::Initialize(textureHandle, position, size);

	textureSize_ = Vector2{ 512.0f,512.0f };
	sprite_->SetTextureSize(textureSize_);
	sprite_->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });

}

void TimeUI::Update()
{

	int remainingTime = GameTimeSystem::GetInstance()->GetRemainingSeconds();

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

void TimeUI::ImGuiDraw()
{

	BaseUI::ImGuiDraw();

}
