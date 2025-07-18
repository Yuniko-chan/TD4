#pragma once
#include "CoursePolygon.h"
#include "../../Engine/Object/MeshObject.h"

struct CourseImportData;

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

	void Initialize(LevelData::MeshData* data, CourseImportData* courseData);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw() override;

	/// <summary>
	/// 三角ポリゴン取得
	/// </summary>
	/// <returns></returns>
	std::vector<CoursePolygon>* GetCoursePolygonsAdress() { return &coursePolygons_; }

	/// <summary>
	/// コーステクスチャハンドル取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCourseTextureHandle() const { return courseTextureHandle_; }

private: // メンバ変数

	// 三角ポリゴン
	std::vector<CoursePolygon> coursePolygons_;

	// コーステクスチャ
	uint32_t courseTextureHandle_;

};

