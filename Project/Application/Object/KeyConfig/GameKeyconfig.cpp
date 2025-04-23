#include "GameKeyconfig.h"
#include "../../../Engine/Input/Input.h"

GameKeyconfig* GameKeyconfig::GetInstance()
{
	static GameKeyconfig instance;
	return &instance;
}

void GameKeyconfig::Initialize()
{
	//---キーマウ向けバインド---//
	// 操作
	playerKeyConfig_.binds.behind = DIK_S;
	playerKeyConfig_.binds.front = DIK_W;
	playerKeyConfig_.binds.left = DIK_A;
	playerKeyConfig_.binds.right = DIK_D;

	// アクション
	playerKeyConfig_.binds.jumpAction = DIK_SPACE;
	playerKeyConfig_.binds.rideAction = DIK_T;

	//---パッド向けキーバインド---//
	playerKeyConfig_.padBinds.jumpAction = XINPUT_GAMEPAD_A;
	playerKeyConfig_.padBinds.rideAction = XINPUT_GAMEPAD_B;

	// 入力
	input_ = Input::GetInstance();
}

void GameKeyconfig::Update()
{
	if (input_->GetJoystickConnected()) {
		playerKeyConfig_.leftStick = input_->GetLeftAnalogstick();
		playerKeyConfig_.rightStick = input_->GetRightAnalogstick();

		playerKeyConfig_.configs.jumpAction = input_->TriggerJoystick(playerKeyConfig_.padBinds.jumpAction);
		playerKeyConfig_.configs.rideAction = input_->TriggerJoystick(playerKeyConfig_.padBinds.rideAction);

	}
	else {
		// PRESS処理
		playerKeyConfig_.configs.behind = input_->PushKey(playerKeyConfig_.binds.behind);
		playerKeyConfig_.configs.front = input_->PushKey(playerKeyConfig_.binds.front);
		playerKeyConfig_.configs.left = input_->PushKey(playerKeyConfig_.binds.left);
		playerKeyConfig_.configs.right = input_->PushKey(playerKeyConfig_.binds.right);

		// TRIGGER処理
		playerKeyConfig_.configs.rideAction = input_->TriggerKey(playerKeyConfig_.binds.rideAction);
		playerKeyConfig_.configs.jumpAction = input_->TriggerKey(playerKeyConfig_.binds.jumpAction);
	}


}
