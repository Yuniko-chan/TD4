#pragma once
#include <memory>
#include <cstdint>
#include <optional>

class IState;
class StateFactory;

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

	/// <summary>
	/// 現在のステート取得
	/// </summary>
	/// <returns></returns>
	IState* GetCurrentState() { return currentState_.get(); }

protected:
	// ファクトリー
	std::unique_ptr<StateFactory> stateFactory_;
	// ステート
	std::unique_ptr<IState> currentState_;
	std::unique_ptr<IState> tmpState_;	// 仮置き
	// リクエスト
	std::optional<uint32_t> request_;
};
