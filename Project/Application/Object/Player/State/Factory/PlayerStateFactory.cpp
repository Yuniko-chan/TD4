#include "PlayerStateFactory.h"
#include "../PlayerStatesList.h"

IState* PlayerStateFactory::CreateState(uint32_t stateNum)
{
	IState* newState = nullptr;

	switch (stateNum)
	{
	case IPlayerState::kRoot:
		newState = new PlayerRootState();
		break;
	case IPlayerState::kOnFoot:
		newState = new PlayerOnFootState();
		break;
	case IPlayerState::kInVehicle:
		newState = new PlayerInVehicleState();
		break;
	default:
		break;
	}

	return newState;
}
