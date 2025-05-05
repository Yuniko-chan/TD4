#include "PlayerPickupManager.h"

#include "../Player.h"
#include "../../Car/Parts/PartsInterface.h"
#include "../../Car/Manager/VehiclePartsManager.h"
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
			nearPartsName_ = nearParts->GetName();
		}
		else {
			nearPartsName_ = "None";
		}
	}

}

void PlayerPickupManager::Update()
{
}

void PlayerPickupManager::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}
