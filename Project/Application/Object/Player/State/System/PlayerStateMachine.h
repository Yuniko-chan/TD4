#pragma once
#include "../../../State/StateMachine.h"
#include <memory>

class IPlayerState;
class Player;

class PlayerStateMachine : public StateMachine
{
public:
	PlayerStateMachine();
	~PlayerStateMachine() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// リクエスト
	/// </summary>
	/// <param name="requestNum"></param>
	void ChangeRequest(uint32_t requestNum) override;

public:
	void SetPlayer(Player* player) { player_ = player; }
private:
	// プレイヤー
	Player* player_ = nullptr;
};
