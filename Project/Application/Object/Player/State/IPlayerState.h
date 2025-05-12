#pragma once
#include "../../State/IState.h"

class Player;

/// <summary>
/// ステートの基底
/// </summary>
class IPlayerState : public IState
{
public:
	enum StateNumber
	{
		kRoot,	// 通常
		kOnFoot,	// 地面を歩いている
		kInVehicle,	// 車両に乗ってる
		kRideAction,	// 乗るアクション
		kDropOffAction,	// 降りるアクション
		kEndNumber,	// 最後
	};

public:
	// デストラクタ
	virtual ~IPlayerState() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Exit() = 0;

	/// <summary>
	/// 親のプレイヤー設定
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

protected:
	// 親機
	Player* player_ = nullptr;

};
