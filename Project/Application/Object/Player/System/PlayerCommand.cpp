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

	// 親子処理（強引に）
	if (playerTransform_->parent_) {
		playerTransform_->parent_->transform_.translate += moveDirect_ * kDeltaTime_;
	}
	else {
		playerTransform_->transform_.translate += moveDirect_ * kDeltaTime_;
	}
}

void PlayerCommand::MoveCommand()
{
	// 初期化
	moveDirect_ = {};

	const float directValue = 1.0f;

	// 前後
	if (keyConfig_->GetConfig()->front) {
		moveDirect_.z += directValue;
	}
	else if (keyConfig_->GetConfig()->behind) {
		moveDirect_.z -= directValue;
	}
	// 左右
	if (keyConfig_->GetConfig()->left) {
		moveDirect_.x -= directValue;
	}
	else if (keyConfig_->GetConfig()->right) {
		moveDirect_.x += directValue;
	}

	// 方向の正規化
	moveDirect_ = Vector3::Normalize(moveDirect_);
}
