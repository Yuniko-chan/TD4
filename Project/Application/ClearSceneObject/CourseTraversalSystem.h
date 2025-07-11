#pragma once
#include <cstdint>
#include <memory>
#include "CourseTraversalRank.h"

/// <summary>
/// コース踏破システム
/// </summary>
class CourseTraversalSystem
{

public: // 静的関数

	/// <summary>
	/// コース踏破回数設定
	/// </summary>
	/// <param name="courseTraversalNum"></param>
	static void SetCourseTraversalNum(int32_t courseTraversalNum) { courseTraversalNum_ = courseTraversalNum; }

private: //  静的変数

	// コース踏破回数
	static int32_t courseTraversalNum_;

public: // 関数

	/// <summary>
	/// 初期化
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
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

private: // 変数

	// ランク
	std::unique_ptr<CourseTraversalRank> courseTraversalRank_;



};

