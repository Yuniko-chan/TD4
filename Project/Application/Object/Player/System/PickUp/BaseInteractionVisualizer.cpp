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
		//interactObject_->SetIsDraw(isDraw_);
		interactObject_->Update();
	}
}

void BaseInteractionVisualizer::SetIsDraw(bool isDraw) {
	isDraw_ = isDraw;
}

void BaseInteractionVisualizer::AddSpot(std::string name, InteractionSpot* interact)
{
	// 既にあればスキップ
	if (interactionSpots_.contains(name)) {
		return;
	}
	// 追加
	interactionSpots_.emplace(name, interact);
}

InteractionSpot* BaseInteractionVisualizer::FindSpot(const std::string& name)
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

void BaseInteractionVisualizer::RefrashSpot(const std::string& name)
{
	for (auto it = interactionSpots_.begin(); it != interactionSpots_.end(); ++it) {
		(*it).second->SetIsDraw(false);
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

void BaseInteractionVisualizer::Reset()
{
	// 終了処理
	interactObject_->SetIsDraw(false);
	interactObject_ = nullptr;
}

void BaseInteractionVisualizer::SetUp(const Vector3& position, const Vector3& direction)
{
	interactObject_->GetWorldTransformAdress()->transform_.translate = position;
	interactObject_->GetWorldTransformAdress()->direction_ = direction;
}
