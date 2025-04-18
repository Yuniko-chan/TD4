#include "PlayerCommand.h"
#include "../../../Engine/Input/Input.h"

PlayerCommand::PlayerCommand()
{
	// 入力ポインタ
	input_ = Input::GetInstance();
}

void PlayerCommand::Update()
{
	// 移動コマンド
	MoveCommand();
}

void PlayerCommand::MoveCommand()
{
	if (input_->GetJoystickConnected()) {

	}
	else {
		// 前後
		if (input_->PushKey(DIK_W)) {
			moveDirect_.z += 1.0f;
		}
		else if (input_->PushKey(DIK_S)) {
			moveDirect_.z -= 1.0f;
		}
		// 左右
		if (input_->PushKey(DIK_A)) {
			moveDirect_.x -= 1.0f;
		}
		else if (input_->PushKey(DIK_D)) {
			moveDirect_.x += 1.0f;
		}
	}

	// 方向の正規化
	moveDirect_ = Vector3::Normalize(moveDirect_);
}
