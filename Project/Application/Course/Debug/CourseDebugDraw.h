#pragma once
#include "../../../Engine/3D/Line/DrawLine.h"
#include "../Course.h"

/// <summary>
/// コースデバッグ描画
/// </summary>
class CourseDebugDraw
{

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="course">コース</param>
	void Initialize(Course* course);

	/// <summary>
	/// 描画マッピング
	/// </summary>
	/// <param name="drawLine">線描画クラス</param>
	void DrawMap(DrawLine* drawLine);

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

private: // 関数

	/// <summary>
	/// ポリゴン描画マッピング
	/// </summary>
	/// <param name="drawLine">線描画</param>
	/// <param name="positions">位置3点</param>
	void DrawMapPolygon(DrawLine* drawLine, const Vector3 positions[3]);

private: // 変数

	// コース
	Course* course_;

	// 表示するか
	bool isDraw_;

};

