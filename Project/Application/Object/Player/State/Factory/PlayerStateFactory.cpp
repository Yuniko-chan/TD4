#include "PlayerStateFactory.h"
#include "../PlayerStatesList.h"

IState* PlayerStateFactory::CreateState(uint32_t stateNum)
{
	IState* newState = nullptr;

	switch (stateNum)
	{
	case 0:
		newState = new PlayerRootState();
		break;
	default:
		break;
	}

	return newState;
}
