#pragma once
#include "../../../Engine/Math/DeltaTime.h"

/// <summary>
/// フレームでのカウントクラス
/// </summary>
class FrameTimer
{
public:
	FrameTimer();
	FrameTimer(float endFrame);
	~FrameTimer() = default;

public:
	/// <summary>
	/// 開始
	/// </summary>
	/// <param name="endTime">終了時間</param>
	/// <param name="isFrame">フレーム単位か</param>
	void Start(float endTime = 60.0f, bool isFrame = true);
	/// <summary>
	/// 終了（強制終了）
	/// </summary>
	void End();
	/// <summary>
	/// 一時停止
	/// </summary>
	void Stop();
	/// <summary>
	/// 再開
	/// </summary>
	void Resume();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="gameFactor"></param>
	void Update(float gameFactor = 1.0f);
private:
	/// <summary>
	/// 開始フラグの設定
	/// </summary>
	void StartUp();
public: // アクセッサ
	// アクティブ確認
	bool IsActive() { return isActive_; }
	// 終了確認
	bool IsEnd() { return isEnd_ && !isActive_; }
	// 一時停止確認
	bool IsPause() { return isPause_ && !isEnd_; }
	// 現在フレーム
	float GetElapsedFrame() { return elapsedFrame_; }
private:
	float elapsedFrame_ = 0.0f; // 経過フレーム（現在フレーム
	float endFrame_ = 0.0f; // 終了フレーム
	bool isActive_ = false;	// 有効フラグ
	bool isEnd_ = false;	// 終了フラグ
	bool isPause_ = false;	// 一時停止フラグ
};