#include "OutGameSpriteObjectAnimation.h"
#include "OutGameSpriteObject.h"

// アニメーション関数
const std::array<
	std::function<void(OutGameSpriteObject*, OutGameSpriteObjectAnimation*)>,
	OutGameSpriteObjectAnimation::kAnimationIndexOfCount> OutGameSpriteObjectAnimation::kFunctions_ =
{
	OutGameSpriteObjectAnimation::NumberRoulette, // ルーレット
	OutGameSpriteObjectAnimation::Scaling // 拡縮
};

void OutGameSpriteObjectAnimation::NumberRoulette(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation)
{
	animation;
	object->SetCurrentSequenceNumber((object->GetCurrentSequenceNumber() + 1) % object->GetSequentialNumberMax());

}

void OutGameSpriteObjectAnimation::Scaling(OutGameSpriteObject* object, OutGameSpriteObjectAnimation* animation)
{
	
	Vector2 size = object->GetSize();

	size = Ease::Easing(animation->scalingVariable_.easeName,
		size + animation->scalingVariable_.add, 
		size - animation->scalingVariable_.sub, 
		std::fabsf(std::sinf(animation->scalingVariable_.count)));

	animation->scalingVariable_.count = 
		std::fmodf(animation->scalingVariable_.count + animation->scalingVariable_.speed, static_cast<float>(std::numbers::pi) * 2.0f);

	object->GetSprite()->SetSize(size);

}

void OutGameSpriteObjectAnimation::Initialize()
{
	for (uint32_t i = 0; i < kAnimationIndexOfCount; ++i) {
		doesAnimations_[i] = false;
	}

	scalingVariable_.add = { 10.0f,10.0f };
	scalingVariable_.sub = { 10.0f,10.0f };
	scalingVariable_.count = 0.0f;
	scalingVariable_.easeName = Ease::EaseName::EaseInBack;
	scalingVariable_.speed = 0.2f;

}

void OutGameSpriteObjectAnimation::Execute(OutGameSpriteObject* object)
{

	for (uint32_t i = 0; i < kAnimationIndexOfCount; ++i) {
		if (doesAnimations_[i]) {
			kFunctions_[i](object, this);
		}
	}

}
