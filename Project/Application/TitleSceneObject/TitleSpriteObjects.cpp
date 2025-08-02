#include "TitleSpriteObjects.h"

void TitleSpriteObjects::Initialize()
{

	// 白
	const Vector4 kWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 透明度変更番号
	const uint32_t kTransparencyChangeLoopNum = static_cast<uint32_t>(OutGameSpriteObjectAnimation::AnimationIndex::kAnimationIndexTransparencyChangeLoop);
	// 拡張番号
	const uint32_t kScalingLoopNum = static_cast<uint32_t>(OutGameSpriteObjectAnimation::AnimationIndex::kAnimationIndexScalingLoop);

	// タイトルロゴ
	titleLogo_ = std::make_unique<OutGameSpriteObject>();
	titleLogo_->Initialize("Resources/Sprite/Common/title.png",
		Vector2{ 640.0f, 200.0f }, kWhite);

	titleLogo_->GetAnimation()->doesAnimations_[kScalingLoopNum] = true;
	titleLogo_->GetAnimation()->scalingVariable_.easeName = Ease::EaseName::Lerp;
	titleLogo_->GetAnimation()->scalingVariable_.start = titleLogo_->GetSize();
	titleLogo_->GetAnimation()->scalingVariable_.end = titleLogo_->GetSize() + Vector2{ 50.0f,25.0f };

	// ボタンをおしてください
	pleasePressTheButton_ = std::make_unique<OutGameSpriteObject>();
	pleasePressTheButton_->Initialize("Resources/Sprite/Common/pressA.png",
		Vector2{ 640.0f, 600.0f }, kWhite);

	pleasePressTheButton_->GetAnimation()->doesAnimations_[kTransparencyChangeLoopNum] = true;

}

void TitleSpriteObjects::Update()
{

	titleLogo_->Update();
	pleasePressTheButton_->Update();

}

void TitleSpriteObjects::Draw()
{
	
	titleLogo_->Draw();
	pleasePressTheButton_->Draw();

}
