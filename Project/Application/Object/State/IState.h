#pragma once

/// <summary>
/// ステート基底
/// </summary>
class IState
{
public: // 仮想関数
	virtual ~IState() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

};
