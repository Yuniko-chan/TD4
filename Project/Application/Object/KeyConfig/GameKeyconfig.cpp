#include "GameKeyconfig.h"
#include "../../../Engine/Input/Input.h"

GameKeyconfig* GameKeyconfig::GetInstance()
{
	static GameKeyconfig instance;
	return &instance;
}

void GameKeyconfig::Initialize()
{
	// 操作
	playerKeyConfig_.binds.behind = DIK_S;
	playerKeyConfig_.binds.front = DIK_W;
	playerKeyConfig_.binds.left = DIK_A;
	playerKeyConfig_.binds.right = DIK_D;

	// アクション
	playerKeyConfig_.binds.jumpAction = DIK_SPACE;
	playerKeyConfig_.binds.rideAction = DIK_RETURN;

	// 入力
	input_ = Input::GetInstance();
}

void GameKeyconfig::Update()
{
	if (input_->GetJoystickConnected()) {
		playerKeyConfig_.leftStick = input_->GetLeftAnalogstick();
		playerKeyConfig_.rightStick = input_->GetRightAnalogstick();
	}
	else {

		playerKeyConfig_.configs.behind = input_->PushKey(playerKeyConfig_.binds.behind);
		playerKeyConfig_.configs.front = input_->PushKey(playerKeyConfig_.binds.front);
		playerKeyConfig_.configs.left = input_->PushKey(playerKeyConfig_.binds.left);
		playerKeyConfig_.configs.right = input_->PushKey(playerKeyConfig_.binds.right);
	}


}
