#include "PickupVisualizer.h"
#include "../../../Interact/InteractionSpot.h"

void PickupVisualizer::Update()
{
	// 親があれば設定
	if (parent_) {
		InteractObject_->GetWorldTransformAdress()->transform_.translate = parent_->GetWorldPosition();
		InteractObject_->SetIsDraw(true);
	}
	else {
		InteractObject_->SetIsDraw(false);
	}
}

void PickupVisualizer::SetIsDraw(bool isDraw)
{
	InteractObject_->SetIsDraw(isDraw);
}
