#pragma once
#include <array>
#include "../../Engine/Math/Vector/Vector2.h"
#include "BaseUI.h"
#include "ManualUI.h"
#include "TimeUI.h"
#include "TutorialUI.h"
#include "TutorialCheckUI.h"
#include "ClockUI.h"

/// <summary>
/// UIマネージャー
/// </summary>
class UIManager
{

public: // サブクラス

	// テクスチャ一覧
	enum TextureIndex {
		kTextureIndexManual,
		kTextureIndexNum,
		kTextureIndexClock,
		kTextureIndexTutorial,
		kTextureIndexTutorialCheck,
		kTextureIndexOfCount
	};

	// UI一覧
	enum UIIndex {
		kUIIndexManual,
		kUIIndexTimeOneHundred,
		kUIIndexTimeTen,
		kUIIndexTimeOne,
		kUIIndexClock,
		kUIIndexTutorial,
		kUIIndexTutorialCheckEngine,
		kUIIndexTutorialCheckTire,
		kUIIndexTutorialCheckFrame,
		kUIIndexOfCount
	};

	// クラス一覧
	enum ClassIndex {
		kClassIndexBase, // ベース
		kClassIndexManual,
		kClassIndexTime,
		kClassIndexTimeClock,
		kClassIndexTutorial,
		kClassIndexTutorialCheck,
		kClassIndexOfCount
	};

	// UI作成データ構造体
	struct UICreateData
	{
		TextureIndex textureIndex; // テクスチャ
		ClassIndex classIndex; // クラス
		Vector2 position; // 位置
		Vector2 size; // 大きさ
	};

private: // 定数

	// テクスチャファイルネーム(Resouce/の次から)
	const std::array<std::string, TextureIndex::kTextureIndexOfCount> kTextureFileNames_ =
	{
		"UI/manual.png",
		"Sprite/Common/number.png",
		"UI/clock.png",
		"UI/tutorial.png",
		"UI/check.png",
	};

	// UIネーム
	const std::array<std::string, UIIndex::kUIIndexOfCount> kUINames_ =
	{
		"manual",
		"timeOneHundred",
		"timeTen",
		"timeOne",
		"clock",
		"tutorial",
		"tutorialCheckEngine",
		"tutorialCheckTire",
		"tutorialCheckFrame",
	};

	// UI作成データ
	const std::array<UICreateData, UIIndex::kUIIndexOfCount> kUICreateDatas_ =
	{
		UICreateData{ kTextureIndexManual, kClassIndexManual, {1100.0f, 570.0f}, {320.0f,240.0f} },
		UICreateData{ kTextureIndexNum, kClassIndexTime, {640.0f - 96.0f,60.0f}, {96.0f,96.0f} },
		UICreateData{ kTextureIndexNum, kClassIndexTime, {640.0f, 60.0f}, {96.0f,96.0f} },
		UICreateData{ kTextureIndexNum, kClassIndexTime, {640.0f + 96.0f,60.0f}, {96.0f,96.0f} },
		UICreateData{ kTextureIndexClock, kClassIndexTimeClock, {640.0f - 96.0f - 96.0f,60.0f}, {96.0f,96.0f} },
		UICreateData{ kTextureIndexTutorial, kClassIndexTutorial, {192.0f + 20.0f , 360.0f}, {384.0f,512.0f} },
		UICreateData{ kTextureIndexTutorialCheck, kClassIndexTutorialCheck, {365.0f , 240.0f}, {100.0f,100.0f}},
		UICreateData{ kTextureIndexTutorialCheck, kClassIndexTutorialCheck, {365.0f , 380.0f}, {100.0f,100.0f} },
		UICreateData{ kTextureIndexTutorialCheck, kClassIndexTutorialCheck, {365.0f , 505.0f}, {100.0f,100.0f} },
	};

public: // メンバ関数

	/// <summary>
	///	初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// Imgui描画
	/// </summary>
	void ImGuiDraw();

	void DisEnable();
private: // メンバ関数

	/// <summary>
	/// UI作成
	/// </summary>
	/// <param name="index">クラス番号</param>
	/// <returns>UI</returns>
	BaseUI* CreateUI(ClassIndex index);

private: // メンバ変数

	// テクスチャハンドル
	std::array<uint32_t, TextureIndex::kTextureIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<BaseUI>, UIIndex::kUIIndexOfCount> uis_;

	// imGui
	int imGuiMode_;

};