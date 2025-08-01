#include "PickupVisualizer.h"
#include "../../../Interact/InteractionSpot.h"
#include "../../../../../Engine/GlobalVariables/GlobalVariables.h"

void PickupVisualizer::Update()
{
	if (interactObject_) {

		// 親子
		if (parent_) { interactObject_->GetWorldTransformAdress()->SetParent(parent_); }
		else { interactObject_->GetWorldTransformAdress()->SetParent(nullptr); }

	}

	// リフレッシュ
	Refresh();
}

void PickupVisualizer::RefrashSpot(const std::string& name)
{
	// 描画フラグリセット
	for (auto it = interactionSpots_.begin(); it != interactionSpots_.end(); ++it) {
		(*it).second->SetIsDraw(false);
	}
	// 名前取得（一部）
	std::string targetName = "";
	if (name.length() >= 5) {
		targetName = name.substr(name.length() - 5);
	}

	//---ピックアップ系---//
	if (targetName == "Point") {
		if (name == "EnginePickupPoint")
		{
			interactObject_ = FindSpot("PickEngineSpot");
		}
		else if (name == "ArmorPickupPoint")
		{
			interactObject_ = FindSpot("PickArmorSpot");
		}
		else if (name == "TirePickupPoint")
		{
			interactObject_ = FindSpot("PickTireSpot");
		}
		// スケールの初期化
		Vector3 scale = GlobalVariables::GetInstance()->GetVector3Value("VehiclePartsInfo", "PickupPointScale");

		interactObject_->GetWorldTransformAdress()->transform_.scale = scale;
	}
	//---パーツ系---//
	else {
		// タイヤ
		if (name == "TireParts") {
			interactObject_ = FindSpot("PickTireSpot");
		}
		// アーマー
		else if (name == "ArmorFrameParts") {
			interactObject_ = FindSpot("PickArmorSpot");
		}
		// エンジン
		else if (name == "EngineParts") {
			interactObject_ = FindSpot("PickEngineSpot");
		}
		// コア
		else if (name == "VehicleCore") {
			interactObject_ = FindSpot("PickArmorSpot");
		}
		// スケールの初期化
		Vector3 scale = GlobalVariables::GetInstance()->GetVector3Value("VehiclePartsInfo", "PickupScale");

		interactObject_->GetWorldTransformAdress()->transform_.scale = scale;
	}

	// 共通処理
	if (interactObject_) {
		// 色の設定
		Vector3 color = GlobalVariables::GetInstance()->GetVector3Value("VehiclePartsInfo", "PickupColor");
		float alpha = GlobalVariables::GetInstance()->GetFloatValue("VehiclePartsInfo", "PickupAlpha");
		interactObject_->SetColor(color);
		interactObject_->SetAlpha(alpha);

		interactObject_->GetWorldTransformAdress()->transform_.translate = Vector3(0.0f, 0.0f, 0.0f);
		interactObject_->GetWorldTransformAdress()->direction_ = Vector3(0.0f, 0.0f, 1.0f);
		
		interactObject_->SetIsDraw(true);
	}
}
