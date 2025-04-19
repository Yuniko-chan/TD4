#pragma once
#include <memory>
#include <cstdint>

class IState;

class StateMachine
{
public:
	// デストラクタ
	virtual ~StateMachine() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 変更リクエスト
	/// </summary>
	/// <param name="requestNum"></param>
	virtual void ChangeRequest(uint32_t requestNum) = 0;

private:
	// ステート
	std::unique_ptr<IState> currentState_;

};
