#pragma once
#include "../Utility/Common/SingletonModule.h"
#include "../../AudioManager/GameAudioManager.h"

/// <summary>
/// ダメージ確認
/// </summary>
class OnHitCheck : public SingletonModule<OnHitCheck>
{

public:
	OnHitCheck();
	~OnHitCheck() = default;
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
	/// ダメージがあった
	/// </summary>
	void Hit() { isHit_ = true; }

	/// <summary>
	/// オーディオマネージャー設定
	/// </summary>
	/// <param name="audioManager"></param>
	void SetAudioManager(GameAudioManager* audioManager) { audioManager_ = audioManager; }

private:

	// ダメージがあった
	bool isHit_;

	// オーディオマネージャー
	GameAudioManager* audioManager_;

	// クールタイム
	float cooltime_;

};

