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
		// 親があれば設定
		if (parent_) {
			interactObject_->SetIsDraw(true);
		}
		else {
			interactObject_->SetIsDraw(false);
		}
		interactObject_->Update();
	}
}
