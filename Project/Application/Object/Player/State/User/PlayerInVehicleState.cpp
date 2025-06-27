#include "PlayerInVehicleState.h"
#include "../../Player.h"
#include "../../../Utility/Calc/CameraHelper.h"
#include "../../../Car/VehicleCore.h"
#include "../../../Engine/Math/DeltaTime.h"

void PlayerInVehicleState::Initialize()
{
	player_->SettingParent();
	// 回転角初期化
	player_->GetWorldTransformAdress()->transform_.rotate = {};
	player_->GetWorldTransformAdress()->direction_ = Vector3(0.0f, 0.0f, 1.0f);

	player_->GetCore()->SetIsDrive(true);

	// カメラの設定
	UpdateCameraOffset();
}

void PlayerInVehicleState::Update()
{
	// 切り替え
	if (player_->GetCommand()->ActionCommand() && !player_->GetPickUpManager()->IsPartsHold()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kDropOffAction);
	}

	UpdateCameraOffset();
	// 車両への入力処理
	player_->GetCore()->GetDriveSystem()->InputAccept(player_->GetCommand()->GetKeyConfig());
}

void PlayerInVehicleState::Exit()
{
	// 親子関係解除＋座標修正
	Vector3 wolrdPosition = player_->GetWorldTransformAdress()->GetWorldPosition();
	Vector3 parentRotate = player_->GetWorldTransformAdress()->parent_->transform_.rotate;
	player_->GetWorldTransformAdress()->SetParent(nullptr);
	player_->GetWorldTransformAdress()->transform_.translate = wolrdPosition;
	player_->GetWorldTransformAdress()->transform_.rotate.y = parentRotate.y;

	player_->GetCore()->SetIsDrive(false);

	// カメラ
	static_cast<FollowCamera*>(player_->GetCameraManager()->FindCamera("Follow"))->SetZoomOutOffset(0.0f);

}

void PlayerInVehicleState::UpdateCameraOffset()
{
	// カメラ
	FollowCamera* camera = static_cast<FollowCamera*>(player_->GetCameraManager()->FindCamera("Follow"));

	// ズームアウト計算
	std::pair<int, int> grid = player_->GetCore()->GetConstructionSystem()->GetMaxGridSize();
	const float kOffsetRatio = 3.0f;	// レート
	const int kGridThreshold = 3;	// 閾値
	CameraHelper cameraHelper;	// ヘルパー
	float offset = cameraHelper.CreateGridSizeOffset(grid, kGridThreshold, kOffsetRatio);
	// オフセット設定
	camera->SetZoomOutOffset(offset);
}
