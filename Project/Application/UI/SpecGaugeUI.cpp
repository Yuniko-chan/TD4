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

}
