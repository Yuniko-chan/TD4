#include "PickupVisualizer.h"
#include "../../../Interact/InteractionSpot.h"

void PickupVisualizer::Update()
{
	if (parent_) {
		InteractObject_->GetWorldTransformAdress()->SetParent(parent_);
		//InteractObject_->GetWorldTransformAdress()->transform_.translate = parent_->GetWorldPosition();
		//InteractObject_->GetWorldTransformAdress()->direction_ = parent_->direction_;
	}
	else {
		InteractObject_->GetWorldTransformAdress()->SetParent(nullptr);
	}
	// リフレッシュ
	Refresh();
}

void PickupVisualizer::SetIsDraw(bool isDraw)
{
	InteractObject_->SetIsDraw(isDraw);
}
