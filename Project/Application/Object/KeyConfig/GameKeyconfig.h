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
	// アクションキー
	T jumpAction;	// ジャンプキー
	T rideAction;	// 乗るキー
	T interactAction;	// 拾う・置くキー

	// 方向のキー
	T front;	// 前
	T behind;	// 後ろ
	T left;	// 左
	T right;	// 右

	// カメラ回転キー
	T rotateLeft;	// 左
	T rotateRight;	// 右
};

/// <summary>
/// バインド
/// </summary>
struct KeyConfig
{
	PlayerKey<bool> configs{};	// 状態
	PlayerKey<uint16_t> binds{};	// バインド
	PlayerKey<uint16_t> padBinds{};	// パッドのバインド
	Vector2 leftStick = {};	// 左スティック
	Vector2 rightStick = {};	// 右スティックの状態
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

	Vector2* GetLeftStick() { return &playerKeyConfig_.leftStick; }
	Vector2* GetRightStick() { return &playerKeyConfig_.rightStick; }
private:
	// プレイヤーのキー
	KeyConfig playerKeyConfig_;
	// 入力
	Input* input_ = nullptr;
};
