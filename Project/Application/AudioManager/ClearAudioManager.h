#pragma once
#include "../../Engine/Audio/AudioManager.h"

/// <summary>
/// クリアオーディオ名前群
/// </summary>
enum ClearAudioNameIndex {
	kClearAudioNameIndexBGM, // BGM
	kClearAudioNameIndexPushButton, // ボタンを押したとき
	kClearAudioNameIndexOfCount // 数
};

/// <summary>
/// クリアオーディオマネージャー
/// </summary>
class ClearAudioManager :
    public AudioManager
{


public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ClearAudioManager();

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
	std::array<AudioData, ClearAudioNameIndex::kClearAudioNameIndexOfCount> audioDatas_;

	// ファイル名
	std::array<const std::string, ClearAudioNameIndex::kClearAudioNameIndexOfCount> audioNames_ =
	{
		"BGM/resultBGM.mp3",
		"Soundeffect/PressButton_SE.mp3",
	};

};

