#pragma once
#include "../../Engine/Audio/AudioManager.h"

/// <summary>
/// タイトルオーディオ名前群
/// </summary>
enum TitleAudioNameIndex {
	kTitleBGM, // BGM
	kTitlePushButton, // ボタンを押したとき
	kTitleAudioNameIndexOfCount // 数
};

class TitleAudioManager :
    public AudioManager
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleAudioManager();

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
	std::array<AudioData, TitleAudioNameIndex::kTitleAudioNameIndexOfCount> audioDatas_;

	// ファイル名
	std::array<const std::string, TitleAudioNameIndex::kTitleAudioNameIndexOfCount> audioNames_ =
	{
		"BGM/SampleBGM.mp3",
		"Soundeffect/PressButton.mp3",
	};

};

