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

void AttachVisualizer::Reset()
{
	// 終了処理
	interactObject_->SetIsDraw(false);
	interactObject_ = nullptr;
}

void AttachVisualizer::SetUp(const Vector3& position, const Vector3& direction)
{
	interactObject_->GetWorldTransformAdress()->transform_.translate = position;
	interactObject_->GetWorldTransformAdress()->direction_ = direction;
}

void AttachVisualizer::Update(const Vector2Int& key)
{
	if (interactObject_) {
		// キーが初期じゃなければ
		if (key == Vector2Int(0, 0)) { parent_ = nullptr; }
		else { 
			parent_ = interactObject_->GetWorldTransformAdress();
		}
		// スケールの初期化
		if (interactObject_->GetWorldTransformAdress()->transform_.scale != Vector3(1.0f, 1.0f, 1.0f)) {
			interactObject_->GetWorldTransformAdress()->transform_.scale = { 1.0f,1.0f,1.0f };
		}
	}
	// リフレッシュ
	Refresh();
}
