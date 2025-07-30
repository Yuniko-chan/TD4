#pragma once
#include "../../Engine/Audio/AudioManager.h"

/// <summary>
/// ゲームオーディオ名前群
/// </summary>
enum GameAudioNameIndex {
	kGameAudioNameIndexBGM, // BGM
	kGameAudioNameIndexAttachment, // パーツをくっつける音
	kGameAudioNameIndexEngine, // エンジンの音
	kGameAudioNameIndexExplosion, // 爆発の音
	kGameAudioNameIndexGrab, // もち上げる音 （PlayerPickupManager）
	kGameAudioNameIndexOfCount // 数
};

/// <summary>
/// ゲームオーディオマネージャー
/// </summary>
class GameAudioManager :
    public AudioManager
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameAudioManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="audioIndex">オーディオの名前</param>
	/// <returns>再生中のサウンドデータの番号</returns>
	void PlayWave(uint32_t audioIndex) override;

private: // メンバ変数

	// データ
	std::array<AudioData, GameAudioNameIndex::kGameAudioNameIndexOfCount> audioDatas_;

	// ファイル名
	std::array<const std::string, GameAudioNameIndex::kGameAudioNameIndexOfCount> audioNames_ =
	{
		"BGM/gameBGM.mp3",
		"Soundeffect/Attachment_SE.mp3",
		"Soundeffect/engine_SE.mp3",
		"Soundeffect/explosion_SE.mp3",
		"Soundeffect/grab_SE.mp3",
	};

};

