#include "PlayerCommand.h"
#include "../../../Engine/Input/Input.h"

PlayerCommand::PlayerCommand()
{
	// 入力ポインタ
	input_ = Input::GetInstance();
}

void PlayerCommand::Update()
{

}

void PlayerCommand::MoveCommand()
{
	if (input_->GetJoystickConnected()) {

	}
	else {
		
	}
}
