#include "PlayerPickupManager.h"

#include "../Player.h"
#include "../../Car/Parts/PartsInterface.h"
#include "../../Car/Manager/VehiclePartsManager.h"
#include "../../Utility/Calc/TransformHelper.h"

#include "../../../Engine/2D/ImguiManager.h"

PlayerPickupManager::PlayerPickupManager()
{
}

void PlayerPickupManager::ImGuiDraw()
{
	ImGui::Text(nearPartsName_.c_str());

	if (ImGui::Button("FindNearPart")) {
		Car::IParts* nearParts = partsManager_->FindNearParts(player_->GetWorldTransformAdress()->GetWorldPosition());
		if (nearParts) {
			const Vector3 direct = nearParts->GetWorldTransformAdress()->GetWorldPosition() - player_->GetWorldTransformAdress()->GetWorldPosition();
			if (player_->GetFrontChecker()->FrontCheck(direct)) {
				nearPartsName_ = "Front";
			}
			else {
				nearPartsName_ = "NotFront";
			}
		}
		else {
			nearPartsName_ = "None";
		}
	}
	if (ImGui::Button("CatchNearParts")) {
		Car::IParts* nearParts = partsManager_->FindNearParts(player_->GetWorldTransformAdress()->GetWorldPosition());
		if (nearParts) {
			nearPartsName_ = nearParts->GetName();
			holdParts_ = nearParts;
			//const Vector3 local = holdParts_->GetWorldTransformAdress()->GetWorldPosition() - player_->GetWorldTransformAdress()->GetWorldPosition();
			const Vector3 localOffset = Vector3(0.0f, 0.0f, 2.0f);
			holdParts_->GetWorldTransformAdress()->SetParent(player_->GetWorldTransformAdress());
			holdParts_->GetWorldTransformAdress()->transform_.translate = localOffset;
			holdParts_->GetWorldTransformAdress()->transform_.rotate = {};
		}
		else {
			nearPartsName_ = "None";
		}
	}
	if (ImGui::Button("ReleaseParts")) {
		nearPartsName_ = "None";
		// ローカル座標→ワールド座標に変換
		//const Vector3 parentRotate = holdParts_->GetWorldTransformAdress()->parent_->transform_.rotate;
		holdParts_->GetWorldTransformAdress()->transform_ = TransformHelper::DetachWithWorldTransform(holdParts_->GetWorldTransformAdress());
		holdParts_->GetWorldTransformAdress()->SetParent(nullptr);
		// パーツの管理を削除
		holdParts_ = nullptr;
	}
}

void PlayerPickupManager::Update()
{
}

void PlayerPickupManager::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}

void PlayerPickupManager::InteractParts()
{
	if (holdParts_) {
		// パーツの位置再設定
		holdParts_->GetWorldTransformAdress()->transform_ = TransformHelper::DetachWithWorldTransform(holdParts_->GetWorldTransformAdress());
		holdParts_->GetWorldTransformAdress()->SetParent(nullptr);
		// パーツの管理を削除
		holdParts_ = nullptr;
	}
	else {
		Car::IParts* nearParts = partsManager_->FindNearParts(player_->GetWorldTransformAdress()->GetWorldPosition());
		if (nearParts) {
			const Vector3 direct = nearParts->GetWorldTransformAdress()->GetWorldPosition() - player_->GetWorldTransformAdress()->GetWorldPosition();
			if (player_->GetFrontChecker()->FrontCheck(direct)) {
				holdParts_ = nearParts;
				const Vector3 localOffset = Vector3(0.0f, 0.0f, 2.0f);
				holdParts_->GetWorldTransformAdress()->SetParent(player_->GetWorldTransformAdress());
				holdParts_->GetWorldTransformAdress()->transform_.translate = localOffset;
				holdParts_->GetWorldTransformAdress()->transform_.rotate = {};
			}
		}
	}
}
