#include "PickupVisualizer.h"
#include "../../../Interact/InteractionSpot.h"

void PickupVisualizer::Update()
{
	if (interactObject_) {

		// 親子
		if (parent_) { interactObject_->GetWorldTransformAdress()->SetParent(parent_); }
		else { interactObject_->GetWorldTransformAdress()->SetParent(nullptr); }

		interactObject_->GetWorldTransformAdress()->transform_.scale = Vector3(2.0f, 2.0f, 2.0f);

	}

	// リフレッシュ
	Refresh();
}

InteractionSpot* PickupVisualizer::FindSpot(const std::string& name)
{
	if (interactionSpots_.contains(name)) {
		for (auto it = interactionSpots_.begin(); it != interactionSpots_.end(); ++it) {
			if (name == it->first) {
				return it->second;
			}
		}
	}
	return nullptr;
}

void PickupVisualizer::RefrashSpot(const std::string& name)
{
	// 
	for (auto it = interactionSpots_.begin(); it != interactionSpots_.end(); ++it) {
		if (interactObject_) {
			if (interactObject_->GetName() != (*it).second->GetName()) {
				(*it).second->SetIsDraw(false);
			}
		}
		else {
			(*it).second->SetIsDraw(false);
		}
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
}

void PickupVisualizer::AddSpot(std::string name, InteractionSpot* interact)
{
	// 既にあればスキップ
	if (interactionSpots_.contains(name)) {
		return;
	}
	// 追加
	interactionSpots_.emplace(name, interact);
}

void PickupVisualizer::SetUp(const Vector3& position, const Vector3& direction)
{
	interactObject_->GetWorldTransformAdress()->transform_.translate = position;
	interactObject_->GetWorldTransformAdress()->direction_ = direction;
}

//void PickupVisualizer::SetIsDraw(bool isDraw)
//{
//	interactObject_->SetIsDraw(isDraw);
//}
