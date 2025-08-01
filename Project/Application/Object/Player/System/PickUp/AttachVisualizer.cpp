#include "AttachVisualizer.h"
#include "../../../Interact/InteractionSpot.h"
#include "../../../../../Engine/GlobalVariables/GlobalVariables.h"

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
		// 色変更
		Vector3 color = GlobalVariables::GetInstance()->GetVector3Value("VehiclePartsInfo", "AttachColor");
		float alpha = GlobalVariables::GetInstance()->GetFloatValue("VehiclePartsInfo", "AttachAlpha");
		interactObject_->SetColor(color);
		interactObject_->SetAlpha(alpha);
		// スケールの初期化
		Vector3 scale = GlobalVariables::GetInstance()->GetVector3Value("VehiclePartsInfo", "AttachScale");
		if (interactObject_->GetWorldTransformAdress()->transform_.scale != Vector3(scale)) {
			interactObject_->GetWorldTransformAdress()->transform_.scale = scale;
		}

		interactObject_->SetIsDraw(true);
	}
	// リフレッシュ
	Refresh();
}
