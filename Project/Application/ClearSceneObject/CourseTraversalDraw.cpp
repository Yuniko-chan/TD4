#include "CourseTraversalDraw.h"
#include "../../Engine/base/Texture/TextureManager.h"

const Vector2 CourseTraversalDraw::kNumberTextureSize_ = { 64.0f,64.0f };

void CourseTraversalDraw::Initialize(int32_t courseTraversalNum)
{

	// 数字の大きさ
	const Vector2 kNumberSize = { 128.0f,128.0f };
	// 白
	const Vector4 kWhite = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 踏破数、文字列
	traversalNumString_ = std::make_unique<OutGameSpriteObject>();
	traversalNumString_->Initialize("Resources/Sprite/Result/traversalNumString.png", 
		Vector2{ 256.0f, 256.0f }, kWhite);

	// 踏破数、数10の位
	traversalNumTenthPlace_ = std::make_unique<OutGameSpriteObject>();
	traversalNumTenthPlace_->Initialize("Resources/Sprite/Common/number.png", 
		Vector2{ 512.0f + 256.0f - kNumberSize.x / 2.0f, 256.0f }, kWhite, kNumberSize, kNumberTextureSize_);
	// ルーレット
	traversalNumTenthPlace_->GetAnimation()->doesAnimations_[0] = true;

	// 踏破数、数1の位
	traversalNumOnePlace_ = std::make_unique<OutGameSpriteObject>();
	traversalNumOnePlace_->Initialize("Resources/Sprite/Common/number.png",
		Vector2{ 512.0f + 256.0f + kNumberSize.x / 2.0f, 256.0f }, kWhite, kNumberSize, kNumberTextureSize_);
	// ルーレット
	traversalNumOnePlace_->GetAnimation()->doesAnimations_[0] = true;

	// 踏破ランク、文字列
	traversalRankString_ = std::make_unique<OutGameSpriteObject>();
	traversalRankString_->Initialize("Resources/Sprite/Result/traversalRankString.png",
		Vector2{ 256.0f,512.0f }, kWhite);

	// 踏破ランク、ランク
	raversalRank_ = std::make_unique<OutGameSpriteObject>();
	raversalRank_->Initialize("Resources/Sprite/Result/traversalRank.png",
		{ 512.0f + 256.0f,512.0f }, kWhite, kNumberSize, kNumberTextureSize_);
	// ルーレット
	raversalRank_->GetAnimation()->doesAnimations_[0] = true;

	// コース踏破数
	courseTraversalNum_ = courseTraversalNum;

}

void CourseTraversalDraw::Update()
{

	// 踏破数、文字列
	traversalNumString_->Update();
	traversalNumTenthPlace_->Update();
	traversalNumOnePlace_->Update();
	traversalRankString_->Update();
	raversalRank_->Update();

}

void CourseTraversalDraw::Draw()
{

	// 踏破数、文字列
	traversalNumString_->Draw();
	// 踏破数、数10の位
	traversalNumTenthPlace_->Draw();
	// 踏破数、数1の位
	traversalNumOnePlace_->Draw();

	// 踏破ランク、文字列
	traversalRankString_->Draw();

	// 踏破ランク、ランク
	raversalRank_->Draw();

}
