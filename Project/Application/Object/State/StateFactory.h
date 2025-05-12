#pragma once
#include <memory>
#include <cstdint>

class IState;

class StateFactory
{
public:
	virtual ~StateFactory() = default;
	/// <summary>
	/// ステート作成
	/// </summary>
	/// <param name="stateNum"></param>
	/// <returns></returns>
	virtual IState* CreateState(uint32_t stateNum) = 0;

};
