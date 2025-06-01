#pragma once
#include "CameraLists.h"

#include <map>
#include <optional>

class GameCameraManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="elapsedTime"></param>
	void Update(float elapsedTime = 0.0f);

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

public:
	// カメラ検索
	BaseCamera* FindCamera(std::string tag);
	// カメラ取得
	BaseCamera* GetActiveCamera();
	// 変更
	void SetRequest(ActiveCamera request) { request_ = request; }

	void Acception();

private:
	// カメラのマップ
	std::map<std::string, std::unique_ptr<BaseCamera>> cameras_;
	// 選択中のカメラ
	ActiveCamera activeCamera_ = ActiveCamera::kOverhead;
	std::optional<ActiveCamera> request_ = std::nullopt;
};
