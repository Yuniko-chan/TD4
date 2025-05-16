#include "GameKeyconfig.h"
#include "../../../Engine/Input/Input.h"

void GameKeyconfig::Initialize()
{
	//---キーマウ向けバインド---//
	// 操作
	playerKeyConfig_.binds.behind = DIK_S;
	playerKeyConfig_.binds.front = DIK_W;
	playerKeyConfig_.binds.left = DIK_A;
	playerKeyConfig_.binds.right = DIK_D;

	playerKeyConfig_.binds.rotateLeft = DIK_Q;
	playerKeyConfig_.binds.rotateRight = DIK_E;

	// アクション
	playerKeyConfig_.binds.jumpAction = DIK_SPACE;
	playerKeyConfig_.binds.rideAction = DIK_T;
	playerKeyConfig_.binds.interactAction = DIK_F;

	// アクセル・ブレーキ
	playerKeyConfig_.binds.accel = DIK_UPARROW;
	playerKeyConfig_.binds.brake = DIK_DOWNARROW;

	//---パッド向けキーバインド---//
	playerKeyConfig_.padBinds.jumpAction = XINPUT_GAMEPAD_A;
	playerKeyConfig_.padBinds.rideAction = XINPUT_GAMEPAD_B;
	playerKeyConfig_.padBinds.interactAction = XINPUT_GAMEPAD_RIGHT_SHOULDER;

	playerKeyConfig_.padBinds.accel = XINPUT_GAMEPAD_X;
	playerKeyConfig_.padBinds.brake = XINPUT_GAMEPAD_Y;

	// 入力
	input_ = Input::GetInstance();
}

void GameKeyconfig::Update()
{
	// 初期化処理
	playerKeyConfig_.leftStick = {};
	playerKeyConfig_.rightStick = {};

	// PADの入力
	if (input_->GetJoystickConnected()) {
		playerKeyConfig_.leftStick = input_->GetLeftAnalogstick();
		playerKeyConfig_.rightStick = input_->GetRightAnalogstick();

		playerKeyConfig_.configs.jumpAction = input_->TriggerJoystick((uint8_t)playerKeyConfig_.padBinds.jumpAction);
		playerKeyConfig_.configs.rideAction = input_->TriggerJoystick((uint8_t)playerKeyConfig_.padBinds.rideAction);
		playerKeyConfig_.configs.interactAction = input_->TriggerJoystick((uint8_t)playerKeyConfig_.padBinds.interactAction);
		playerKeyConfig_.configs.accel = input_->TriggerJoystick((uint8_t)playerKeyConfig_.padBinds.accel);
		playerKeyConfig_.configs.brake = input_->TriggerJoystick((uint8_t)playerKeyConfig_.padBinds.brake);

	}
	// KEYBOARDの入力
	else {
		// PRESS処理
		playerKeyConfig_.configs.behind = input_->PushKey((uint8_t)playerKeyConfig_.binds.behind);
		playerKeyConfig_.configs.front = input_->PushKey((uint8_t)playerKeyConfig_.binds.front);
		playerKeyConfig_.configs.left = input_->PushKey((uint8_t)playerKeyConfig_.binds.left);
		playerKeyConfig_.configs.right = input_->PushKey((uint8_t)playerKeyConfig_.binds.right);
		playerKeyConfig_.configs.rotateLeft = input_->PushKey((uint8_t)playerKeyConfig_.binds.rotateLeft);
		playerKeyConfig_.configs.rotateRight = input_->PushKey((uint8_t)playerKeyConfig_.binds.rotateRight);
		playerKeyConfig_.configs.accel = input_->PushKey((uint8_t)playerKeyConfig_.binds.accel);
		playerKeyConfig_.configs.brake = input_->PushKey((uint8_t)playerKeyConfig_.binds.brake);
		//---スティックに変換---//
		// 左
		if (playerKeyConfig_.configs.behind) {
			playerKeyConfig_.leftStick.y = -1.0f;
		}
		else if (playerKeyConfig_.configs.front) {
			playerKeyConfig_.leftStick.y = 1.0f;
		}
		if (playerKeyConfig_.configs.left) {
			playerKeyConfig_.leftStick.x = -1.0f;
		}
		else if (playerKeyConfig_.configs.right) {
			playerKeyConfig_.leftStick.x = 1.0f;
		}
		// 右
		if (playerKeyConfig_.configs.rotateRight) {
			playerKeyConfig_.rightStick.x = 1.0f;
		}
		else if (playerKeyConfig_.configs.rotateLeft) {
			playerKeyConfig_.rightStick.x = -1.0f;
		}

		// TRIGGER処理
		playerKeyConfig_.configs.rideAction = input_->TriggerKey((uint8_t)playerKeyConfig_.binds.rideAction);
		playerKeyConfig_.configs.jumpAction = input_->TriggerKey((uint8_t)playerKeyConfig_.binds.jumpAction);
		playerKeyConfig_.configs.interactAction = input_->TriggerKey((uint8_t)playerKeyConfig_.binds.interactAction);
	}
}
