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

	playerTransform_->transform_.translate += moveDirect_ * kDeltaTime_;
}

void PlayerCommand::MoveCommand()
{
	// 初期化
	moveDirect_ = {};

	// 前後
	if (keyConfig_->GetConfig()->front) {
		moveDirect_.z += 1.0f;
	}
	else if (keyConfig_->GetConfig()->behind) {
		moveDirect_.z -= 1.0f;
	}
	// 左右
	if (keyConfig_->GetConfig()->left) {
		moveDirect_.x -= 1.0f;
	}
	else if (keyConfig_->GetConfig()->right) {
		moveDirect_.x += 1.0f;
	}

	// 方向の正規化
	moveDirect_ = Vector3::Normalize(moveDirect_);
}
