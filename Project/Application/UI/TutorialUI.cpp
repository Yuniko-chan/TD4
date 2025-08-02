#include "TutorialUI.h"

void TutorialUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
{

	BaseUI::Initialize(textureHandle, position, size);

	tutorial_ = Tutorial::GetInstance();

	isDisplay_ = true;

}

void TutorialUI::Update()
{

	if (!isDisplay_) {
		return;
	}

	if (tutorial_->IsDrive()) {
		sprite_->SetIsInvisible(true);
	}
	else {
		sprite_->SetIsInvisible(false);
	}

	if (tutorial_->GetIsEnd()) {
		isDisplay_ = false;
		sprite_->SetIsInvisible(true);
	}

}

void TutorialUI::ImGuiDraw()
{

	BaseUI::ImGuiDraw();

}
