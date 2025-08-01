#include "CourseTraversalDraw.h"
#include "../../Engine/base/Texture/TextureManager.h"

const Vector2 CourseTraversalDraw::kNumberTextureSize_ = { 512.0f,512.0f };

const std::array<float, CourseTraversalDraw::kFlowIndexOfCount> CourseTraversalDraw::kFlowSwitchingTime_ = 
{ 0.0f, 2.0f, 4.0f};

void CourseTraversalDraw::Initialize(int32_t courseTraversalNum, int32_t rankNum)
{

	// 数字の大きさ
	const Vector2 kNumberSize = { 128.0f,128.0f };
	// 白
	const Vector4 kWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ルーレット番号
	const uint32_t kRouletteNum = static_cast<uint32_t>(OutGameSpriteObjectAnimation::AnimationIndex::kAnimationIndexNumberRoulette);
	// 透明度
	const uint32_t kTransparencyChangeNum = static_cast<uint32_t>(OutGameSpriteObjectAnimation::AnimationIndex::kAnimationIndexTransparencyChangeLoop);
	// 中心位置
	const Vector2 kCenter = { 640.0f, 360.0f };
	// 踏破数位置追加
	const Vector2 kTraversalNumAdd = { 250.0f, -100.0f };

	// 踏破数、文字列
	traversalNumString_ = std::make_unique<OutGameSpriteObject>();
	traversalNumString_->Initialize("Resources/Sprite/Result/traversalNumString.png", 
		Vector2{ kCenter.x - kTraversalNumAdd.x, kCenter.y + kTraversalNumAdd.y }, kWhite);

	// 踏破数、数10の位
	traversalNumTenthPlace_ = std::make_unique<OutGameSpriteObject>();
	traversalNumTenthPlace_->Initialize("Resources/Sprite/Common/number.png", 
		Vector2{ kCenter.x + kTraversalNumAdd.x - kNumberSize.x / 2.0f, kCenter.y + kTraversalNumAdd.y }, kWhite, kNumberSize, kNumberTextureSize_);
	// ルーレット
	traversalNumTenthPlace_->GetAnimation()->doesAnimations_[kRouletteNum] = true;

	// 踏破数、数1の位
	traversalNumOnePlace_ = std::make_unique<OutGameSpriteObject>();
	traversalNumOnePlace_->Initialize("Resources/Sprite/Common/number.png",
		Vector2{ kCenter.x + kTraversalNumAdd.x + kNumberSize.x / 2.0f, kCenter.y + kTraversalNumAdd.y }, kWhite, kNumberSize, kNumberTextureSize_);
	// ルーレット
	traversalNumOnePlace_->GetAnimation()->doesAnimations_[kRouletteNum] = true;

	// 踏破ランク、ランク
	raversalRank_ = std::make_unique<OutGameSpriteObject>();
	raversalRank_->Initialize("Resources/Sprite/Result/traversalRank.png",
		{ kCenter.x , kCenter.y + 100.0f }, kWhite, kNumberSize, kNumberTextureSize_);
	// ルーレット
	raversalRank_->GetAnimation()->doesAnimations_[kRouletteNum] = true;

	// ボタンをおしてください
	pleasePressTheButton_ = std::make_unique<OutGameSpriteObject>();
	pleasePressTheButton_->Initialize("Resources/Sprite/Result/traversalNumString.png",
		Vector2{ kCenter.x, kCenter.y + 250.0f }, kWhite);
	// 透明度
	pleasePressTheButton_->GetAnimation()->doesAnimations_[kTransparencyChangeNum] = true;

	// コース踏破数
	courseTraversalNum_ = courseTraversalNum;
	// ランク数字
	rankNum_ = 4 - rankNum;

	// 流れ
	flow_ = kFlowIndexCourseTraversal;
	// 流れの切り替わりカウント
	flowCount_ = 0.0f;

}

void CourseTraversalDraw::Update(int32_t courseTraversalNum,int32_t rankNum)
{

	FlowCheck();

#ifdef _DEMO
	courseTraversalNum_ = courseTraversalNum;
	rankNum_ = 4 - rankNum;
	if (flow_ == kFlowIndexWaitingButton) {
		traversalNumTenthPlace_->SetCurrentSequenceNumber(courseTraversalNum_ / 10);
		traversalNumOnePlace_->SetCurrentSequenceNumber(courseTraversalNum_ % 10);
		raversalRank_->SetCurrentSequenceNumber(rankNum_);
	}
#endif // _DEMO

	courseTraversalNum;
	rankNum;

	// 踏破数、文字列
	traversalNumString_->Update();
	traversalNumTenthPlace_->Update();
	traversalNumOnePlace_->Update();
	raversalRank_->Update();
	pleasePressTheButton_->Update();

}

void CourseTraversalDraw::Draw()
{

	// 踏破数、文字列
	traversalNumString_->Draw();
	// 踏破数、数10の位
	traversalNumTenthPlace_->Draw();
	// 踏破数、数1の位
	traversalNumOnePlace_->Draw();
	// 踏破ランク、ランク
	raversalRank_->Draw();
	// ボタンをおしてください
	pleasePressTheButton_->Draw();

}

void CourseTraversalDraw::FlowCheck()
{

	// ルーレット番号
	const uint32_t kRouletteNum = static_cast<uint32_t>(OutGameSpriteObjectAnimation::AnimationIndex::kAnimationIndexNumberRoulette);

	// ボタン待ちまでいっているのでリターン
	if (flow_ == kFlowIndexWaitingButton) {
		return;
	}

	// カウントを増やす
	flowCount_ += kDeltaTime_;
	if (flowCount_ >= kFlowSwitchingTime_[flow_ + 1]) {
		flow_++;

		switch (flow_)
		{
		case kFlowIndexCourseTraversal:
			break;
		case kFlowIndexRaversalRank:
			traversalNumTenthPlace_->GetAnimation()->doesAnimations_[kRouletteNum] = false;
			traversalNumOnePlace_->GetAnimation()->doesAnimations_[kRouletteNum] = false;

			traversalNumTenthPlace_->SetCurrentSequenceNumber(courseTraversalNum_ / 10);
			traversalNumOnePlace_->SetCurrentSequenceNumber(courseTraversalNum_ % 10);

			break;
		case kFlowIndexWaitingButton:
			raversalRank_->GetAnimation()->doesAnimations_[kRouletteNum] = false;

			raversalRank_->SetCurrentSequenceNumber(rankNum_);
			break;
		default:
			break;
		}

	}

}
