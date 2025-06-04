#include "PlayerCommand.h"
#include "../Player.h"
#include "../../KeyConfig/GameKeyconfig.h"
#include "../../Utility/Calc/TransformHelper.h"
#include "../../../Engine/Math/DeltaTime.h"

PlayerCommand::PlayerCommand()
{
	// 入力ポインタ
	keyConfig_ = GameKeyconfig::GetInstance();
}

void PlayerCommand::Initialize(Player* player)
{
	playerTransform_ = player->GetWorldTransformAdress();
}

void PlayerCommand::Update()
{
	// 移動コマンド
	MoveCommand();
}

bool PlayerCommand::MoveCommand()
{
	// 初期化
	moveDirect_ = {};
	Vector3 leftStick = { keyConfig_->GetLeftStick()->x ,0.0f,keyConfig_->GetLeftStick()->y };
	moveDirect_.x = keyConfig_->GetLeftStick()->x;
	moveDirect_.z = keyConfig_->GetLeftStick()->y;
	// 正規化
	moveDirect_ = Vector3::Normalize(moveDirect_);
	// 方向入力があればtrue
	return moveDirect_.x != 0.0f || moveDirect_.y != 0.0f || moveDirect_.z != 0.0f;
}

void PlayerCommand::RotateCommand()
{
	// 回転の適応
	Vector3 transformDirect = Vector3::Normalize(Matrix4x4::TransformNormal(moveDirect_, playerTransform_->rotateMatrix_));
	// 回転角
	Vector2 rotate = Vector2::Normalize(Vector2(keyConfig_->GetRightStick()->x, keyConfig_->GetRightStick()->y));
	// ワールドトランスフォームに適応
	const float rotateSpeed = GlobalVariables::GetInstance()->GetFloatValue("Player", "CameraRotateSpeed");
	const float rotateRatio = 1.0f / rotateSpeed;
	playerTransform_->transform_.rotate.y += (rotate.x * rotateRatio);
	// 移動方向ベクトルの更新
	moveDirect_ = Vector3::Normalize(transformDirect);
}

void PlayerCommand::VectorRotate()
{
	theta_ = 0.0f;
	// 回転速度計算
	const float rotateSpeed = GlobalVariables::GetInstance()->GetFloatValue("Player", "CameraRotateSpeed");
	const float rotateRatio = 1.0f / rotateSpeed;

	if (keyConfig_->GetRightStick()->x > 0) {
		theta_ -= rotateRatio;
	}
	else if (keyConfig_->GetRightStick()->x < 0) {
		theta_ += rotateRatio;
	}
	// X-Z平面上に回転
	Vector3 direct = TransformHelper::XZRotation(playerTransform_->direction_, theta_);
	playerTransform_->direction_ = Vector3::Normalize(direct);
}

void PlayerCommand::StickMoveCommand()
{
	//Vector2 rotate = Vector2::Normalize(Vector2(keyConfig_->GetLeftStick()->x, keyConfig_->GetLeftStick()->y));
	// 向き
	Vector3 direct = moveDirect_;
	// 入力があれば更新
	if (direct != Vector3(0.0f, 0.0f, 0.0f)) {
		// 例外処理
		if (direct == Vector3(0.0f, 0.0f, -1.0f)) {
			direct.x = -0.1f;
			playerTransform_->direction_ = Vector3::Normalize(direct);
		}
		// 通常処理
		else {
			playerTransform_->direction_ = direct;
		}
	}
}

void PlayerCommand::InVehicleRotateCommand()
{
	// 回転の適応
	Vector3 transformDirect = Vector3::Normalize(Matrix4x4::TransformNormal(moveDirect_, playerTransform_->parent_->rotateMatrix_));
	// 回転角
	Vector2 rotate = Vector2::Normalize(Vector2(keyConfig_->GetRightStick()->x, keyConfig_->GetRightStick()->y));
	// ワールドトランスフォームに適応
	const float rotateRatio = 1.0f / 45.0f;
	playerTransform_->parent_->transform_.rotate.y += (rotate.x * rotateRatio);
	// 移動方向ベクトルの更新
	moveDirect_ = Vector3::Normalize(transformDirect);
}

bool PlayerCommand::ActionCommand()
{
	return keyConfig_->GetConfig()->rideAction;
}

bool PlayerCommand::InteractCommand()
{
	return keyConfig_->GetConfig()->interactAction;
}
