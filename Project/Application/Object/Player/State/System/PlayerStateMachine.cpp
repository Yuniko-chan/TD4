#include "PlayerStateMachine.h"
#include "../../Player.h"
#include "../IPlayerState.h"
#include "../Factory/PlayerStateFactory.h"

PlayerStateMachine::PlayerStateMachine()
{
}

PlayerStateMachine::~PlayerStateMachine()
{
}

void PlayerStateMachine::Initialize()
{
	stateFactory_ = std::make_unique<PlayerStateFactory>();
	ChangeRequest(IPlayerState::kOnFoot);
}

void PlayerStateMachine::Update()
{
	// 変更リクエスト
	if (request_) {
		// 終了関数
		if (currentState_) {
			currentState_->Exit();
		}
		// 初期化
		static_cast<IPlayerState*>(tmpState_.get())->SetPlayer(player_);
		tmpState_->Initialize();
		currentState_ = std::move(tmpState_);

		// リセット
		request_ = std::nullopt;

	}
	// 現在のステート処理
	if (currentState_) {
		currentState_->Update();
	}

}

void PlayerStateMachine::ChangeRequest(uint32_t requestNum)
{
	// リクエスト受け取り
	request_ = requestNum;
	// ステートの作成
	tmpState_.reset(stateFactory_->CreateState(request_.value()));
}
