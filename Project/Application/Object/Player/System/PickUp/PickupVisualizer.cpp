#include "PickupVisualizer.h"
#include "../../../Interact/InteractionSpot.h"

void PickupVisualizer::Update()
{
	// 親子
	if (parent_) { interactObject_->GetWorldTransformAdress()->SetParent(parent_); }
	else { interactObject_->GetWorldTransformAdress()->SetParent(nullptr); }
	// リフレッシュ
	Refresh();
}

void PickupVisualizer::SetIsDraw(bool isDraw)
{
	interactObject_->SetIsDraw(isDraw);
}
