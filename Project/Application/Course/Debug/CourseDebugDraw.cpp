#include "CourseDebugDraw.h"
#include "../../../Engine/2D/ImguiManager.h"

void CourseDebugDraw::Initialize()
{

	// コース
	courses_.clear();

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
	for (std::list<Course*>::iterator itr = courses_.begin(); itr != courses_.end(); ++itr) {
		std::vector<CoursePolygon>* polygons = (*itr)->GetCoursePolygonsAdress();

		Vector3 worldPosition = (*itr)->GetWorldTransformAdress()->GetWorldPosition();

		for (uint32_t i = 0; i < polygons->size(); ++i) {
			// ポリゴンマッピング
			DrawMapPolygon(drawLine, (*polygons)[i].position0 + worldPosition, (*polygons)[i].position1 + worldPosition, (*polygons)[i].position2 + worldPosition);
		}
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

void CourseDebugDraw::SetCourse(Course* course)
{

	courses_.push_back(course);

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
