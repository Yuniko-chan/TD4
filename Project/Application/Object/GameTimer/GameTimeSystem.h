#pragma once

class GameTimeSystem
{
public:
	GameTimeSystem();
	~GameTimeSystem() = default;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:
	/// <summary>
	/// 開始
	/// </summary>
	void Start();
	/// <summary>
	/// 一時停止
	/// </summary>
	void Pause();
	/// <summary>
	/// 再開
	/// </summary>
	void Resume();

private:
	// 経過時間
	float elapsedTime_ = 0.0f;
	// 残り時間
	float remainingTime_ = 0.0f;
	// 稼働中か
	bool isRunning_ = false;
	// 一時停止か
	bool isPause_ = false;
};
