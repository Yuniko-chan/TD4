#include "OutGameSpriteObject.h"
#include "../../Engine/base/Texture/TextureManager.h"

void OutGameSpriteObject::Initialize(const std::string& fileName, const Vector2& position, const Vector4& color)
{

	// DirectXCommon
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	textureHandle_ = TextureManager::Load(fileName, dxCommon);
	position_ = position;
	color_ = color;
	sprite_.reset(Sprite::Create(textureHandle_, position_, color_));
	size_ = sprite_->GetSize();
	projectionRange_ = size_;
	currentSequenceNumber_ = 0;
	sequentialNumberMax_ = static_cast<uint32_t>(sprite_->GetTextureInitSize().x / projectionRange_.x);

	// アニメーション
	animation_ = std::make_unique<OutGameSpriteObjectAnimation>();
	animation_->Initialize();

}

void OutGameSpriteObject::Initialize(const std::string& fileName, const Vector2& position, const Vector4& color, const Vector2& size, const Vector2& projectionRange)
{

	Initialize(fileName, position, color);

	sprite_->SetTextureLeftTop(Vector2{ 0.0f,0.0f });
	sprite_->SetTextureSize(projectionRange);
	projectionRange_ = projectionRange;
	sprite_->SetSize(size);
	size_ = size;
	sequentialNumberMax_ = static_cast<uint32_t>(sprite_->GetTextureInitSize().x / projectionRange_.x);

}

void OutGameSpriteObject::Update()
{

	//	アニメーション
	animation_->Execute(this);

	// 連番処理
	sprite_->SetTextureLeftTop(Vector2{ projectionRange_.x * currentSequenceNumber_,0.0f });

}

void OutGameSpriteObject::Draw()
{

	sprite_->Draw();

}
