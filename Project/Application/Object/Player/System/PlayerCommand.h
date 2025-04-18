#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"

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
	// 入力クラス
	Input* input_ = nullptr;
	// 方向
	Vector3 moveDirect_ = {};
};
