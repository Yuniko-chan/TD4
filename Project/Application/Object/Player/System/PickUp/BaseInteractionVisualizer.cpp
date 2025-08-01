#include "BaseInteractionVisualizer.h"
#include "../../Player.h"
#include "../../../Interact/InteractionSpot.h"

void BaseInteractionVisualizer::Initialize(Player* owner)
{
	// オーナーの設定
	SetOwner(owner);
}

void BaseInteractionVisualizer::Update()
{

}

void BaseInteractionVisualizer::Refresh()
{
	if (interactObject_) {
		interactObject_->SetIsDraw(isDraw_);
		interactObject_->Update();
	}
}

void BaseInteractionVisualizer::SetIsDraw(bool isDraw) {
	isDraw_ = isDraw;
}