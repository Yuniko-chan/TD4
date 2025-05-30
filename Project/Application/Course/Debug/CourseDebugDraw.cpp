#include "CourseDebugDraw.h"
#include "../../../Engine/2D/ImguiManager.h"

void CourseDebugDraw::Initialize(Course* course)
{

	// コース
	course_ = course;

	// 表示するか
	isDraw_ = false;

}

void CourseDebugDraw::DrawMap(DrawLine* drawLine)
{

	// 描画するか
	if (!isDraw_) {
		return;
	}

	// コースマッピング
	std::vector<CoursePolygon>* polygons = course_->GetCoursePolygonsAdress();
	for (uint32_t i = 0; i < polygons->size(); ++i) {
	
		// ポリゴンマッピング
		DrawMapPolygon(drawLine, (*polygons)[i].position0, (*polygons)[i].position1, (*polygons)[i].position2);
	
	}

}

void CourseDebugDraw::ImGuiDraw()
{

#ifdef _DEMO
	ImGui::Begin("CourseDebugDraw");
	ImGui::Checkbox("描画するか", &isDraw_);
	ImGui::End();
#endif // _DEMO

}

void CourseDebugDraw::DrawMapPolygon(DrawLine* drawLine, const Vector3& position0, const Vector3& position1, const Vector3& position2)
{

	LineForGPU lineForGPU = {};
	lineForGPU.color[0] = { 1.0f,1.0f,1.0f,1.0f };
	lineForGPU.color[1] = { 1.0f,1.0f,1.0f,1.0f };

	lineForGPU.position[0] = position0;
	lineForGPU.position[1] = position1;
	drawLine->Map(lineForGPU);

	lineForGPU.position[0] = position1;
	lineForGPU.position[1] = position2;
	drawLine->Map(lineForGPU);

	lineForGPU.position[0] = position2;
	lineForGPU.position[1] = position0;
	drawLine->Map(lineForGPU);

}
