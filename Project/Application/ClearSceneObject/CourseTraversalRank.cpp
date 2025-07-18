#include "CourseTraversalRank.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Engine/2D/ImguiManager.h"

const std::array<std::string, CourseTraversalRank::kRankNumMax_> CourseTraversalRank::kRankNames_ 
= { "D", "C", "B", "A", "S" };

void CourseTraversalRank::Initialize(uint32_t courseTraversalNum)
{

	// ボーダーライン
	for (size_t i = 0; i < kRankNumMax_; ++i) {
		rankBorderline_[i] = kRankNumMax_ - (i + 1);
	}

	// グローバル変数を登録する
	RegisteringGlobalVariables();

	// グローバル変数を適用する
	ApplyGlobalVariables();

	// ランク
	DetermineRank(courseTraversalNum);

}

void CourseTraversalRank::Update(uint32_t courseTraversalNum)
{

#ifdef _DEMO
	DetermineRank(courseTraversalNum);
	ApplyGlobalVariables();
#endif
	courseTraversalNum;

}

void CourseTraversalRank::ImGuiDraw()
{

	ImGui::Text("ランク：%s", &rank_);

}

void CourseTraversalRank::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "CourseTraversal";
	std::string key = "";

	// ボーダーライン
	for (size_t i = 0; i < kRankNumMax_; ++i) {
		key = std::format("rankBorderline{}", i);
		rankBorderline_[i] = globalVariables->GetIntValue(groupName, key);
	}

}

void CourseTraversalRank::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "CourseTraversal";
	std::string key = "";

	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	// ボーダーライン
	for (size_t i = 0; i < kRankNumMax_; ++i) {
		key = std::format("rankBorderline{}", i);
		rankBorderline_[i] = globalVariables->GetIntValue(groupName, key);
		globalVariables->AddItem(groupName, key, static_cast<int>(rankBorderline_[i]));
	}

}

void CourseTraversalRank::DetermineRank(uint32_t courseTraversalNum)
{

	rank_ = "";

	for (int32_t i = 0; i < kRankNumMax_; ++i) {
		// ボーダーライン以上ならそのランクで決定
		if (courseTraversalNum >= rankBorderline_[i]) {
			rank_ = kRankNames_[i];
			rankNum_ = i;
			break;
		}
	}

}
