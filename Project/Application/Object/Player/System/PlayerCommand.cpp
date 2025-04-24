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

	moveDirect_.x = keyConfig_->GetLeftStick()->x;
	moveDirect_.z = keyConfig_->GetLeftStick()->y;

	// 方向の正規化
	moveDirect_ = Vector3::Normalize(moveDirect_);

	Vector2 rotate = Vector2::Normalize(Vector2(keyConfig_->GetRightStick()->x, keyConfig_->GetRightStick()->y));

	playerTransform_->transform_.rotate.y += rotate.x;

	// 方向入力があればtrue
	return moveDirect_.x != 0.0f || moveDirect_.y != 0.0f || moveDirect_.z != 0.0f;
}

bool PlayerCommand::ActionCommand()
{
	return keyConfig_->GetConfig()->rideAction;
}
