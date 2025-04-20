#pragma once
#include "../../../State/StateMachine.h"
#include <memory>

class IPlayerState;

class PlayerStateMachine : public StateMachine
{
public:
	PlayerStateMachine();
	~PlayerStateMachine() override;

	void Initialize() override;
	void Update() override;
	void ChangeRequest(uint32_t requestNum) override;

	//void Initialize();
	//void Update();

private:

};
