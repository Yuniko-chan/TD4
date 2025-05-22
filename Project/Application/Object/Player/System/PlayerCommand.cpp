#include "PlayerCommand.h"
#include "../Player.h"
#include "../../KeyConfig/GameKeyconfig.h"
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
	const float rotateRatio = 1.0f / 45.0f;
	playerTransform_->transform_.rotate.y += (rotate.x * rotateRatio);
	// 移動方向ベクトルの更新
	moveDirect_ = Vector3::Normalize(transformDirect);
}

void PlayerCommand::VectorRotate()
{
	theta_ = 0.0f;

	if (keyConfig_->GetRightStick()->x > 0) {
		theta_ -= 0.01f;
	}
	else if (keyConfig_->GetRightStick()->x < 0) {
		theta_ += 0.01f;
	}

	float cosT = std::cosf(theta_);
	float sinT = std::sinf(theta_);

	Vector3 direct = {
		playerTransform_->direction_.x * cosT - playerTransform_->direction_.z * sinT,
		playerTransform_->direction_.y,
		playerTransform_->direction_.x * sinT + playerTransform_->direction_.z * cosT,
	};
	
	playerTransform_->direction_ = Vector3::Normalize(direct);
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
