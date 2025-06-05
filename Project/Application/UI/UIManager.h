#pragma once
#include <array>
#include "../../Engine/Math/Vector/Vector2.h"
#include "BaseUI.h"
#include "ManualUI.h"

/// <summary>
/// UIマネージャー
/// </summary>
class UIManager
{

public: // サブクラス

	// テクスチャ一覧
	enum TextureIndex {
		manual,
		kTextureIndexOfCount
	};

	// UI一覧
	enum UIIndex {
		manualUIindex,
		kUIIndexOfCount
	};

	// クラス一覧
	enum ClassIndex {
		kClassIndexBase, // ベース
		manualUIClass,
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

	// テクスチャファイルネーム(Resouce/UI/の次から)
	const std::array<std::string, TextureIndex::kTextureIndexOfCount> kTextureFileNames_ =
	{
		"manual.png"
	};

	// UIネーム
	const std::array<std::string, UIIndex::kUIIndexOfCount> kUINames_ =
	{
		"manual",
	};

	// UI作成データ
	const std::array<UICreateData, UIIndex::kUIIndexOfCount> kUICreateDatas_ =
	{
		UICreateData{ manual, manualUIClass, {1050.0f,550.0f}, {256.0f,150.0f} },
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

