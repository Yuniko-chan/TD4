#include "PickupVisualizer.h"
#include "../../../Interact/InteractionSpot.h"

void PickupVisualizer::Update()
{
	if (interactObject_) {

		// 親子
		if (parent_) { interactObject_->GetWorldTransformAdress()->SetParent(parent_); }
		else { interactObject_->GetWorldTransformAdress()->SetParent(nullptr); }

		interactObject_->GetWorldTransformAdress()->transform_.scale = Vector3(1.5f, 1.5f, 1.5f);

	}

	// リフレッシュ
	Refresh();
}

void PickupVisualizer::RefrashSpot(const std::string& name)
{
	// 
	for (auto it = interactionSpots_.begin(); it != interactionSpots_.end(); ++it) {
		(*it).second->SetIsDraw(false);
		//if (interactObject_) {
		//	if (interactObject_->GetWorldTransformAdress()->GetWorldPosition() != (*it).second->GetWorldTransformAdress()->GetWorldPosition()) {
		//		(*it).second->SetIsDraw(false);
		//	}
		//}
		//else {
		//	(*it).second->SetIsDraw(false);
		//}
	}
	// タイヤ
	if (name == "TireParts") {
		interactObject_ = FindSpot("TireSpot");
	}
	// アーマー
	else if (name == "ArmorFrameParts") {
		interactObject_ = FindSpot("ArmorSpot");
	}
	// エンジン
	else if (name == "EngineParts") {
		interactObject_ = FindSpot("EngineSpot");
	}
	// コア
	else if (name == "VehicleCore") {
		interactObject_ = FindSpot("ArmorSpot");
	}
}
