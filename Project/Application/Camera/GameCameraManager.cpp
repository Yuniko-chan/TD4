#include "GameCameraManager.h"
#include "CameraLists.h"
#include "../../Engine/2D/ImguiManager.h"

void GameCameraManager::Initialize()
{
	// 追従カメラ
	cameras_.emplace("Follow", std::make_unique<FollowCamera>());
	// 俯瞰カメラ
	cameras_.emplace("Overhead", std::make_unique<OverheadCamera>());
	// カメラの更新
	for (std::map<std::string, std::unique_ptr<BaseCamera>>::iterator it = cameras_.begin();
		it != cameras_.end(); ++it) {
		(*it).second->Initialize();
	}
}

void GameCameraManager::Update(float elapsedTime)
{
	// リクエストの受付
	if (request_) {
		activeCamera_ = request_.value();
		request_ = std::nullopt;
	}

	// カメラの更新
	for (std::map<std::string, std::unique_ptr<BaseCamera>>::iterator it = cameras_.begin();
		it != cameras_.end(); ++it) {
		(*it).second->Update(elapsedTime);
	}
}

void GameCameraManager::ImGuiDraw()
{
	ImGui::Begin("CameraManager");

	// 追従
	ImGui::BeginChild("Follow", ImVec2(300, 150), true);

	static_cast<FollowCamera*>(FindCamera("Follow"))->ImGuiDraw();

	ImGui::EndChild();

	// 俯瞰
	ImGui::BeginChild("Overhead", ImVec2(300, 150), true);

	ImGui::EndChild();

	ImGui::End();
}

BaseCamera* GameCameraManager::FindCamera(std::string tag)
{
	// イテレータ取得
	std::map<std::string, std::unique_ptr<BaseCamera>>::iterator it = cameras_.find(tag);
	// なければエラー
	assert(it != cameras_.end());
	// 見つけたイテレータからポインタを取得
	return static_cast<BaseCamera*>((*it).second.get());
}

BaseCamera* GameCameraManager::GetActiveCamera()
{
	// 選択中のカメラを返すように
	switch (activeCamera_)
	{
	case ActiveCamera::kDebug:
		return FindCamera("Debug");
		break;
	case ActiveCamera::kFollow:
		return FindCamera("Follow");
		break;
	case ActiveCamera::kOverhead:
		return FindCamera("Overhead");
		break;
	case ActiveCamera::kMaxCount:
		
		break;
	}
	// なければnull
	return nullptr;
}
