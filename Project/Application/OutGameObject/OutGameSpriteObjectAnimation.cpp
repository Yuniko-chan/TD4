#include "OutGameSpriteObjectAnimation.h"
#include "OutGameSpriteObject.h"

// アニメーション関数
const std::array<
	std::function<void(OutGameSpriteObject*, OutGameSpriteObjectAnimation*)>,
	OutGameSpriteObjectAnimation::kAnimationIndexOfCount> OutGameSpriteObjectAnimation::kFunctions_ =
{
	OutGameSpriteObjectAnimation::NumberRoulette, // ルーレット
	OutGameSpriteObjectAnimation::Scaling, // 拡縮
	OutGameSpriteObjectAnimation::ScalingLoop, // 拡縮ループ
	OutGameSpriteObjectAnimation::Moving, // 移動
	OutGameSpriteObjectAnimation::MovingLoop // 移動ループ
};

void OutGameSpriteObjectAnimation::NumberRoulette(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation)
{
	animation;
	object->SetCurrentSequenceNumber((object->GetCurrentSequenceNumber() + 1) % object->GetSequentialNumberMax());

}

void OutGameSpriteObjectAnimation::Scaling(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation)
{
	
	if (animation->scalingVariable_.t >= 1.0f) {
		return;
	}

	Vector2 size = object->GetSize();

	size = Ease::Easing(animation->scalingVariable_.easeName,
		animation->scalingVariable_.start,
		animation->scalingVariable_.end,
		animation->scalingVariable_.t);

	animation->scalingVariable_.t = 
		std::clamp(animation->scalingVariable_.t + animation->scalingVariable_.speed, 0.0f, 1.0f);

	object->GetSprite()->SetSize(size);

}

void OutGameSpriteObjectAnimation::ScalingLoop(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation)
{

	Vector2 size = object->GetSize();

	size = Ease::Easing(animation->scalingVariable_.easeName,
		animation->scalingVariable_.start,
		animation->scalingVariable_.end,
		std::fabsf(std::sinf(animation->scalingVariable_.t)));

	animation->scalingVariable_.t =
		std::fmodf(animation->scalingVariable_.t + animation->scalingVariable_.speed, static_cast<float>(std::numbers::pi) * 2.0f);

	object->GetSprite()->SetSize(size);

}

void OutGameSpriteObjectAnimation::Moving(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation)
{

	if (animation->movingVariable_.t >= 1.0f) {
		return;
	}

	Vector2 position = Ease::Easing(animation->movingVariable_.easeName,
		animation->movingVariable_.start,
		animation->movingVariable_.end,
		animation->movingVariable_.t);

	animation->movingVariable_.t =
		std::clamp(animation->movingVariable_.t + animation->movingVariable_.speed, 0.0f, 1.0f);

	object->GetSprite()->SetPosition(position);

}

void OutGameSpriteObjectAnimation::MovingLoop(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation)
{

	Vector2 position = Ease::Easing(animation->movingVariable_.easeName,
		animation->movingVariable_.start,
		animation->movingVariable_.end,
		std::fabsf(std::cosf(animation->movingVariable_.t)));

	animation->movingVariable_.t =
		std::fmodf(animation->movingVariable_.t + animation->movingVariable_.speed, static_cast<float>(std::numbers::pi) * 2.0f);

	object->GetSprite()->SetPosition(position);

}

void OutGameSpriteObjectAnimation::Initialize()
{
	for (uint32_t i = 0; i < kAnimationIndexOfCount; ++i) {
		doesAnimations_[i] = false;
	}

	scalingVariable_.start = { 128.0f,128.0f };
	scalingVariable_.end = { 256.0f,256.0f };
	scalingVariable_.t = 0.0f;
	scalingVariable_.easeName = Ease::EaseName::EaseInBack;
	scalingVariable_.speed = 0.02f;

	movingVariable_.t = 0.0f; // カウント
	movingVariable_.start = { 100.0f,100.0f }; // 始まり
	movingVariable_.end = { 600.0f,600.0f }; // 終わり
	movingVariable_.easeName = Ease::EaseName::Lerp; // イージング方法
	movingVariable_.speed = 0.02f; // スピード

}

void OutGameSpriteObjectAnimation::Execute(OutGameSpriteObject* object)
{

	for (uint32_t i = 0; i < kAnimationIndexOfCount; ++i) {
		if (doesAnimations_[i]) {
			kFunctions_[i](object, this);
		}
	}

}
