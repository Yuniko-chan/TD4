#pragma once
#include "../../../State/StateMachine.h"
#include "../../../Utility/Common/OwnerComponent.h"
#include <memory>

class IPlayerState;
class Player;

class PlayerStateMachine : public StateMachine, public OwnerComponent<Player>
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

};
