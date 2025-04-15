#include "PostEffectSystem.h"

void PostEffectSystem::Initialize()
{

	// ポストエフェクト
	postEffect_ = PostEffect::GetInstance();
	// DirectXCommon
	dxCommon_ = DirectXCommon::GetInstance();
	// ウインドウスプライト
	windowSprite_ = WindowSprite::GetInstance();

}

void PostEffectSystem::Update()
{

}

void PostEffectSystem::Execute()
{

}
