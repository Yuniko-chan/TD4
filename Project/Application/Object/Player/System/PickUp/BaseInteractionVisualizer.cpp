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
	// 親があれば設定
	if (parent_) {
		InteractObject_->SetIsDraw(true);
	}
	else {
		InteractObject_->SetIsDraw(false);
	}
}
