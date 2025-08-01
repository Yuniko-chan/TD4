#include "AttachVisualizer.h"
#include "../../../Interact/InteractionSpot.h"

void AttachVisualizer::Initialize(Player* owner)
{
	BaseInteractionVisualizer::Initialize(owner);
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

		interactObject_->SetIsDraw(true);
	}
	// リフレッシュ
	Refresh();
}
