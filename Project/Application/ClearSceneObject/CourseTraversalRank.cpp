#include "CourseTraversalRank.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"

const std::array<std::string, CourseTraversalRank::kRankNumMax_> CourseTraversalRank::kRankNames_ 
= { "A", "B", "C" };

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
#endif

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

	for (size_t i = 0; i < kRankNumMax_; ++i) {
		// ボーダーライン以上ならそのランクで決定
		if (courseTraversalNum >= rankBorderline_[i]) {
			rank_ = kRankNames_[i];
			break;
		}
	}

}
