#include "OnHitCheck.h"
#include "../../../Engine/Math/DeltaTime.h"

OnHitCheck::OnHitCheck()
{
}

void OnHitCheck::Initialize()
{

	isHit_ = false;
	cooltime_ = 0.0f;
}

void OnHitCheck::Update()
{

	cooltime_ -= kDeltaTime_;
	if (cooltime_ < 0.0f) {
		cooltime_ = 0.0f;
		if (isHit_) {
			// 音鳴らす
			audioManager_->PlayWave(kGameAudioNameIndexDamage);
			const float kCooltime = 1.0f;
			cooltime_ = kCooltime;
		}
	}

	isHit_ = false;

}
