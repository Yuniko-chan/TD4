#pragma once
#include "../../../State/StateFactory.h"

class PlayerStateFactory : public StateFactory
{
public:
	/// <summary>
	/// 作成
	/// </summary>
	/// <param name="stateNum"></param>
	/// <returns></returns>
	IState* CreateState(uint32_t stateNum) override;

};
