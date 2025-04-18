#pragma once
#include <memory>

class IPlayerState;

class PlayerStateMachine
{
public:
	PlayerStateMachine();
	~PlayerStateMachine();

	//void Initialize();
	//void Update();

private:
	// 所持ステート
	std::unique_ptr<IPlayerState> currentState_;

};
