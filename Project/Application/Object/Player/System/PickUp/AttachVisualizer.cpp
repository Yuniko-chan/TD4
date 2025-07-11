#include "AttachVisualizer.h"
#include "../../../Interact/InteractionSpot.h"

void AttachVisualizer::Initialize(Player* owner)
{
	BaseInteractionVisualizer::Initialize(owner);
}

void AttachVisualizer::AddSpot(std::string name, InteractionSpot* interact)
{
	// 既にあればスキップ
	if (interactionSpots_.contains(name)) {
		return;
	}
	// 追加
	interactionSpots_.emplace(name, interact);
}

InteractionSpot* AttachVisualizer::FindSpot(const std::string& name)
{
	if (interactionSpots_.contains(name)) {
		for (auto it = interactionSpots_.begin(); it != interactionSpots_.end(); ++it) {
			if (name == (*it).first) {
				return (*it).second;
			}
		}
	}
	return nullptr;
}

void AttachVisualizer::RefrashSpot(const std::string& name)
{
	// タイヤ
	if (name == "TireParts") {
		InteractObject_ = FindSpot("TireSpot");
	}
	// アーマー
	else if (name == "ArmorFrameParts") {
		InteractObject_ = FindSpot("ArmorSpot");
	}
	// エンジン
	else if (name == "EngineParts") {
		InteractObject_ = FindSpot("EngineSpot");
	}
}

void AttachVisualizer::Reset()
{
	// 終了処理
	InteractObject_->SetIsDraw(false);
	InteractObject_ = nullptr;
}

void AttachVisualizer::SetUp(const Vector3& position, const Vector3& direction)
{
	InteractObject_->GetWorldTransformAdress()->transform_.translate = position;
	InteractObject_->GetWorldTransformAdress()->direction_ = direction;
	InteractObject_->SetIsDraw(true);
}
