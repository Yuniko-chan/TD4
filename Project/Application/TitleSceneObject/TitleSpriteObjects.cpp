#include "TitleSpriteObjects.h"

void TitleSpriteObjects::Initialize()
{

	// 白
	const Vector4 kWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ルーレット番号
	const uint32_t kTransparencyChangeLoopNum = static_cast<uint32_t>(OutGameSpriteObjectAnimation::AnimationIndex::kAnimationIndexTransparencyChangeLoop);


	// タイトルロゴ
	titleLogo_ = std::make_unique<OutGameSpriteObject>();
	titleLogo_->Initialize("Resources/Sprite/Result/traversalNumString.png",
		Vector2{ 640.0f, 200.0f }, kWhite);

	// ボタンをおしてください
	pleasePressTheButton_ = std::make_unique<OutGameSpriteObject>();
	pleasePressTheButton_->Initialize("Resources/Sprite/Result/traversalNumString.png",
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
