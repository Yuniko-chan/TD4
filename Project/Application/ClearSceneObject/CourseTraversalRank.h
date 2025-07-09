#pragma once
#include <cstdint>
#include <array>
#include <string>

/// <summary>
/// コース踏破ランク
/// </summary>
class CourseTraversalRank
{

public: // 定数

	// ランクの数
	static const size_t kRankNumMax_ = 3;

	// ランクの名前
	static const std::array<std::string, kRankNumMax_> kRankNames_;

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="courseTraversalNum">コース踏破数</param>
	void Initialize(uint32_t courseTraversalNum);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="courseTraversalNum">コース踏破数</param>
	void Update(uint32_t courseTraversalNum);

private: // 関数

	/// <summary>
	/// グローバル変数を適用する
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// グローバル変数を登録する
	/// </summary>
	void RegisteringGlobalVariables();

	/// <summary>
	/// ランクを決める
	/// </summary>
	/// <param name="courseTraversalNum">コース踏破数</param>
	void DetermineRank(uint32_t courseTraversalNum);

private: // 変数

	// ランクのボーダーライン
	std::array<size_t, kRankNumMax_> rankBorderline_;

	// ランク
	std::string rank_;

};

