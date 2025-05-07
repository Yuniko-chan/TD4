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

public: // 
	/// <summary>
	/// アクションコマンド
	/// </summary>
	/// <returns></returns>
	bool ActionCommand();
	/// <summary>
	/// インタラクト
	/// </summary>
	/// <returns></returns>
	bool InteractCommand();

	/// <summary>
	/// 操作コマンド
	/// </summary>
	bool MoveCommand();
	/// <summary>
	/// 方向取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetDirect() { return moveDirect_ * kSpeedValue; }

	/// <summary>
	/// 回転処理
	/// </summary>
	void RotateCommand();
	/// <summary>
	/// 車両に乗っている際の回転
	/// </summary>
	void InVehicleRotateCommand();
private:
	// 方向
	Vector3 moveDirect_ = {};
	// プレイヤーのトランスフォーム
	WorldTransform* playerTransform_ = nullptr;
	// 入力クラス
	GameKeyconfig* keyConfig_ = nullptr;

	const float kSpeedValue = 5.0f;
};
