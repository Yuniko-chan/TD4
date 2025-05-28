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
		DrawMapPolygon(drawLine, (*polygons)[i].positions);
	
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

void CourseDebugDraw::DrawMapPolygon(DrawLine* drawLine, const Vector3 positions[3])
{

	LineForGPU lineForGPU = {};
	lineForGPU.color[0] = { 1.0f,1.0f,1.0f,1.0f };
	lineForGPU.color[1] = { 1.0f,1.0f,1.0f,1.0f };

	lineForGPU.position[0] = positions[0];
	lineForGPU.position[1] = positions[1];
	drawLine->Map(lineForGPU);

	lineForGPU.position[0] = positions[1];
	lineForGPU.position[1] = positions[2];
	drawLine->Map(lineForGPU);

	lineForGPU.position[0] = positions[2];
	lineForGPU.position[1] = positions[0];
	drawLine->Map(lineForGPU);

}
