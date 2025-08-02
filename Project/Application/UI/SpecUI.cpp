#include "SpecUI.h"
#include "../Object/Car/VehicleCore.h"

void SpecUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
{

	BaseUI::Initialize(textureHandle, position, size);

	vehicleCore_ = nullptr;

}

void SpecUI::Update()
{

	sprite_->SetIsInvisible(vehicleCore_->IsDrive());

}
