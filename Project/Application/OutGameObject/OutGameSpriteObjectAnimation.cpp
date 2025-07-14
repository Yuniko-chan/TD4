#include "OutGameSpriteObjectAnimation.h"
#include "OutGameSpriteObject.h"

// アニメーション関数
const std::array<
	std::function<void(OutGameSpriteObject*)>,
	OutGameSpriteObjectAnimation::kAnimationNumberMax_> OutGameSpriteObjectAnimation::kFunctions_ =
{
	OutGameSpriteObjectAnimation::NumberRoulette // ルーレット
};

void OutGameSpriteObjectAnimation::NumberRoulette(OutGameSpriteObject* object)
{

	object->SetCurrentSequenceNumber((object->GetCurrentSequenceNumber() + 1) % object->GetSequentialNumberMax());

}

void OutGameSpriteObjectAnimation::Initialize()
{
	for (uint32_t i = 0; i < kAnimationNumberMax_; ++i) {
		doesAnimations_[i] = false;
	}
}

void OutGameSpriteObjectAnimation::Execute(OutGameSpriteObject* object)
{

	for (uint32_t i = 0; i < kAnimationNumberMax_; ++i) {
		if (doesAnimations_[i]) {
			kFunctions_[i](object);
		}
	}

}
