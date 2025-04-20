#include "PlayerStateFactory.h"
#include "../PlayerStatesList.h"

std::unique_ptr<IState> PlayerStateFactory::CreateState(uint32_t stateNum)
{
	std::unique_ptr<IState> newState;

	switch (stateNum)
	{
	case 0:
		newState = std::make_unique<PlayerRootState>();
		break;
	default:
		break;
	}

	return newState;
}
