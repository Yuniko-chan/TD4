#pragma once

class Input;

/// <summary>
/// 入力の受付クラス
/// </summary>
class PlayerCommand {
public:
	PlayerCommand();
	~PlayerCommand() = default;
	//void Initialize();
	void Update();
	void MoveCommand();

private:
	Input* input_ = nullptr;

};
