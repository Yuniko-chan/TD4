#include "SpecGaugeUI.h"
#include "../Object/Car/VehicleCore.h"

void SpecGaugeUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
{

	BaseUI::Initialize(textureHandle, position, size);

	vehicleCore_ = nullptr;

	spriteSizeMaxX_ = size.x;

	textureSizeMaxX_ = sprite_->GetTextureSize().x;

}

void SpecGaugeUI::Update()
{

	sprite_->SetIsInvisible(vehicleCore_->IsDrive());

	// 映す割合
	float textureSizeRatio = 0.5f;

	switch (gaugeIndex_)
	{
	case SpecGaugeUI::kGaugeIndexSpeed:
		textureSizeRatio = vehicleCore_->GetDriveSystem()->GetDriveEngine()->GetCurrentEngine();
		break;
	case SpecGaugeUI::kGaugeIndexRightTurn:
		textureSizeRatio = vehicleCore_->GetDriveSystem()->GetDriveHandling()->GetCurrentRightTire();
		break;
	case SpecGaugeUI::kGaugeIndexLeftTurn:
		textureSizeRatio = vehicleCore_->GetDriveSystem()->GetDriveHandling()->GetCurrentLeftTire();
		break;
	case SpecGaugeUI::kkGaugeIndexOfCount:
	default:
		assert(0);
		break;
	}

	sprite_->SetPosition(Vector2{ position_.x - (size_.x * (1.0f - textureSizeRatio) * 0.5f) ,position_.y });
	sprite_->SetSize(Vector2{ size_.x * textureSizeRatio ,size_.y });
	sprite_->SetTextureSize(Vector2{textureSize_.x * textureSizeRatio ,textureSize_ .y});

}
