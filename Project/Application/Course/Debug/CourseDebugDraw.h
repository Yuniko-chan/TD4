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
	/// <param name="positions0">位置0</param>
	/// <param name="positions1">位置1</param>
	/// <param name="positions2">位置2</param>
	void DrawMapPolygon(DrawLine* drawLine, const Vector3& position0, const Vector3& position1, const Vector3& position2);

private: // 変数

	// コース
	Course* course_;

	// 表示するか
	bool isDraw_;

};

