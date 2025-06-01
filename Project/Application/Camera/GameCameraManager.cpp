#include "GameCameraManager.h"
#include "CameraLists.h"
#include "../../Engine/2D/ImguiManager.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"

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
		Acception();
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

	static char changeName[256];
	ImGui::InputText("ChangeCamera", changeName, 256);

	// 変更受付
	if (ImGui::Button("Change")) {
		std::string sName = changeName;
		if (sName == "Follow") {
			SetRequest(ActiveCamera::kFollow);
		}
		if (sName == "Overhead") {
			SetRequest(ActiveCamera::kOverhead);
		}
	}

	// 追従
	ImGui::BeginChild("Follow", ImVec2(300, 150), true);

	static_cast<FollowCamera*>(FindCamera("Follow"))->ImGuiDraw();

	ImGui::EndChild();

	// 俯瞰
	ImGui::BeginChild("Overhead", ImVec2(300, 150), true);

	static_cast<OverheadCamera*>(FindCamera("Overhead"))->ImGuiDraw();

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

void GameCameraManager::Acception()
{
	//const float changeFrame = 60.0f;
	// 選択中のカメラを返すように
	switch (activeCamera_)
	{
	case ActiveCamera::kDebug:
		FindCamera("Debug");
		break;
	case ActiveCamera::kFollow:
		static_cast<FollowCamera*>(FindCamera("Follow"))->Accept(GlobalVariables::GetInstance()->GetFloatValue("Player", "RideActionFrame"));
		break;
	case ActiveCamera::kOverhead:
		static_cast<OverheadCamera*>(FindCamera("Overhead"))->Accept(GlobalVariables::GetInstance()->GetFloatValue("Player", "DropOffActionFrame"));
		break;
	}

}
