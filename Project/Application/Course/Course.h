#pragma once
#include "CoursePolygon.h"
#include "../../Engine/Object/MeshObject.h"

/// <summary>
/// コース
/// </summary>
class Course : 
	public MeshObject
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="data">データ</param>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw() override;

private: // メンバ変数

	// 三角ポリゴン（頂点三つ、法線、種類）
	std::vector<CoursePolygon> coursePolygons_;

};

