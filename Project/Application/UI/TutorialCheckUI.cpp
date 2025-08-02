#include "TutorialCheckUI.h"

void TutorialCheckUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
{

	BaseUI::Initialize(textureHandle, position, size);

	tutorial_ = Tutorial::GetInstance();

	isDisplay_ = true;

	tutorialIndex_ = kTutorialIndexEngine;

}

void TutorialCheckUI::Update()
{

	if (!isDisplay_) {
		return;
	}

	if (tutorial_->GetIsEnd()) {
		isDisplay_ = false;
		sprite_->SetIsInvisible(true);
	}

	bool isInvisible_ = true;

	switch (tutorialIndex_)
	{
	case TutorialCheckUI::kTutorialIndexEngine:
		isInvisible_ = !tutorial_->GetEngineCheck();
		break;
	case TutorialCheckUI::kTutorialIndexTire:
		isInvisible_ = !tutorial_->GetTireCheck();
		break;
	case TutorialCheckUI::kTutorialIndexFrame:
		isInvisible_ = !tutorial_->GetFrameCheck();
		break;
	case TutorialCheckUI::kTutorialIndexOfCount:
	default:
		assert(0);
		break;
	}

	sprite_->SetIsInvisible(isInvisible_);

}
