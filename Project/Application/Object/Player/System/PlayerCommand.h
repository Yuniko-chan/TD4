#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Engine/3D/Transform/WorldTransform.h"

class GameKeyconfig;
class Player;

/// <summary>
/// 入力の受付クラス
/// </summary>
class PlayerCommand {
public:
	PlayerCommand();
	~PlayerCommand() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	/// <summary>
	/// 操作コマンド
	/// </summary>
	void MoveCommand();

	

private:
	// 方向
	Vector3 moveDirect_ = {};
	// プレイヤーのトランスフォーム
	WorldTransform* playerTransform_ = nullptr;
	// 入力クラス
	GameKeyconfig* keyConfig_ = nullptr;
};
