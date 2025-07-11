#include "CourseTraversalDraw.h"
#include "../../Engine/base/Texture/TextureManager.h"

const Vector2 CourseTraversalDraw::kNumberTextureSize_ = { 64.0f,64.0f };

void CourseTraversalDraw::Initialize(int32_t courseTraversalNum)
{

	// DirectXCommon
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 数字の大きさ
	const Vector2 kNumberSize = { 128.0f,128.0f };

	// 踏破数、文字列
	traversalNumString_.textureHandle = TextureManager::Load("Resources/Sprite/Result/traversalNumString.png", dxCommon);
	traversalNumString_.position = { 256.0f, 256.0f };
	traversalNumString_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	traversalNumString_.sprite.reset(Sprite::Create(traversalNumString_.textureHandle, traversalNumString_.position, traversalNumString_.color));
	traversalNumString_.size = traversalNumString_.sprite->GetSize();
	traversalNumString_.sprite->SetSize(traversalNumString_.size);

	// 踏破数、数10の位
	traversalNumTenthPlace_.textureHandle = TextureManager::Load("Resources/Sprite/Common/number.png", dxCommon);
	traversalNumTenthPlace_.position = { 512.0f + 256.0f - kNumberSize.x / 2.0f, 256.0f };
	traversalNumTenthPlace_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	traversalNumTenthPlace_.sprite.reset(Sprite::Create(traversalNumTenthPlace_.textureHandle, traversalNumTenthPlace_.position, traversalNumTenthPlace_.color));
	traversalNumTenthPlace_.size = traversalNumTenthPlace_.sprite->GetSize();
	
	traversalNumTenthPlace_.sprite->SetTextureLeftTop(Vector2{ 0.0f,0.0f });
	traversalNumTenthPlace_.sprite->SetTextureSize(kNumberTextureSize_);
	traversalNumTenthPlace_.sprite->SetSize(kNumberSize);

	// 描画のする数字
	traversalNumTenthPlaceRoulette_.drawNumber = 0;
	// ルーレットするか
	traversalNumTenthPlaceRoulette_.duringRoulette = true;

	// 踏破数、数1の位
	traversalNumOnePlace_.textureHandle = TextureManager::Load("Resources/Sprite/Common/number.png", dxCommon);
	traversalNumOnePlace_.position = { 512.0f + 256.0f + kNumberSize.x / 2.0f, 256.0f };
	traversalNumOnePlace_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	traversalNumOnePlace_.sprite.reset(Sprite::Create(traversalNumOnePlace_.textureHandle, traversalNumOnePlace_.position, traversalNumOnePlace_.color));
	traversalNumOnePlace_.size = traversalNumOnePlace_.sprite->GetSize();

	traversalNumOnePlace_.sprite->SetTextureLeftTop(Vector2{ 0.0f,0.0f });
	traversalNumOnePlace_.sprite->SetTextureSize(kNumberTextureSize_);
	traversalNumOnePlace_.sprite->SetSize(kNumberSize);

	// 描画のする数字
	traversalNumOnePlaceRoulette_.drawNumber = 0;
	// ルーレットするか
	traversalNumOnePlaceRoulette_.duringRoulette = true;

	// 踏破ランク、文字列
	traversalRankString_.textureHandle = TextureManager::Load("Resources/Sprite/Result/traversalRankString.png", dxCommon);
	traversalRankString_.position = { 256.0f,512.0f };
	traversalRankString_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	traversalRankString_.sprite.reset(Sprite::Create(traversalRankString_.textureHandle, traversalRankString_.position, traversalRankString_.color));
	traversalRankString_.size = traversalRankString_.sprite->GetSize();
	traversalRankString_.sprite->SetSize(traversalRankString_.size);

	// 踏破ランク、ランク
	raversalRank_.textureHandle = TextureManager::Load("Resources/Sprite/Result/traversalRank.png", dxCommon);
	raversalRank_.position = { 512.0f + 256.0f,512.0f };
	raversalRank_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	raversalRank_.sprite.reset(Sprite::Create(raversalRank_.textureHandle, raversalRank_.position, raversalRank_.color));
	raversalRank_.size = raversalRank_.sprite->GetSize();
	raversalRank_.sprite->SetSize(raversalRank_.size);

	raversalRank_.sprite->SetTextureLeftTop(Vector2{ 0.0f,0.0f });
	raversalRank_.sprite->SetTextureSize(kNumberTextureSize_);
	raversalRank_.sprite->SetSize(kNumberSize);

	// 描画のする数字
	raversalRankRoulette_.drawNumber = 0;
	// ルーレットするか
	raversalRankRoulette_.duringRoulette = true;

	// コース踏破数
	courseTraversalNum_ = courseTraversalNum;

}

void CourseTraversalDraw::Update()
{

	// ルーレット
	NumberRoulette();

}

void CourseTraversalDraw::Draw()
{

	// 踏破数、文字列
	traversalNumString_.sprite->Draw();
	// 踏破数、数10の位
	traversalNumTenthPlace_.sprite->Draw();
	// 踏破数、数1の位
	traversalNumOnePlace_.sprite->Draw();

	// 踏破ランク、文字列
	traversalRankString_.sprite->Draw();

	// 踏破ランク、ランク
	raversalRank_.sprite->Draw();

}

void CourseTraversalDraw::NumberRoulette()
{

	// 10の位
	if (traversalNumTenthPlaceRoulette_.duringRoulette) {
		traversalNumTenthPlaceRoulette_.drawNumber = (traversalNumTenthPlaceRoulette_.drawNumber + 1) % 10;
		traversalNumTenthPlace_.sprite->SetTextureLeftTop(Vector2{ kNumberTextureSize_.x * traversalNumTenthPlaceRoulette_.drawNumber,0.0f });
	}

	// 1の位
	if (traversalNumOnePlaceRoulette_.duringRoulette) {
		traversalNumOnePlaceRoulette_.drawNumber = (traversalNumOnePlaceRoulette_.drawNumber + 1) % 10;
		traversalNumOnePlace_.sprite->SetTextureLeftTop(Vector2{ kNumberTextureSize_.x * traversalNumOnePlaceRoulette_.drawNumber,0.0f });
	}

	// ランク
	if (raversalRankRoulette_.duringRoulette) {
		raversalRankRoulette_.drawNumber = (raversalRankRoulette_.drawNumber + 1) % 10;
		raversalRank_.sprite->SetTextureLeftTop(Vector2{ kNumberTextureSize_.x * raversalRankRoulette_.drawNumber,0.0f });
	}

}
