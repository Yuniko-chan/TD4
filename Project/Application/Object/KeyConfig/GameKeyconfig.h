#pragma once
#include "../../../Engine/Math/Vector/Vector2.h"

class Input;

/// <summary>
/// キーバインド
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
struct PlayerKey
{
	T jumpAction;	// ジャンプキー
	T rideAction;	// 乗るキー

	// 方向のキー
	T front;	// 前
	T behind;	// 後ろ
	T left;	// 左
	T right;	// 右
};

/// <summary>
/// バインド
/// </summary>
struct KeyConfig
{
	PlayerKey<bool> configs;	// 状態
	PlayerKey<uint8_t> binds;	// バインド
	Vector2 leftStick, rightStick;	// スティックの状態
	bool isController = false;	// Controller操作か
};

class GameKeyconfig
{
public: // シングルトン
	static GameKeyconfig* GetInstance();

	GameKeyconfig() = default;
	~GameKeyconfig() = default;
	GameKeyconfig(const GameKeyconfig&) = delete;
	const GameKeyconfig& operator=(const GameKeyconfig&) = delete;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// コンフィグの取得
	/// </summary>
	/// <returns></returns>
	PlayerKey<bool>* GetConfig() { return &playerKeyConfig_.configs; }

private:
	// プレイヤーのキー
	KeyConfig playerKeyConfig_;
	// 入力
	Input* input_ = nullptr;
};
