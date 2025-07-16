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
	/// 開始
	/// </summary>
	/// <param name="limitTime">終了時間（秒数）</param>
	void Start(const float& limitTime);
	/// <summary>
	/// 一時停止
	/// </summary>
	void Pause();
	/// <summary>
	/// 再開
	/// </summary>
	void Resume();
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
private:
	// 残り時間
	float remainingTime_ = 0.0f;
	int remainingSeconds_ = 0;
	// 稼働中か
	bool isRunning_ = false;
	// 一時停止か
	bool isPause_ = false;
	// 終了フラグ
	bool isEnd_ = false;
};
