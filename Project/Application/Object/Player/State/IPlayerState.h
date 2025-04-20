#pragma once
#include "../../State/IState.h"

/// <summary>
/// ステートの基底
/// </summary>
class IPlayerState : public IState
{
public:
	enum StateNumber
	{
		kRoot,	// 通常
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
};
