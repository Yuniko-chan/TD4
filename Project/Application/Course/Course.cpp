#include "Course.h"
#include "CoursePolygonType.h"
#include "../../Engine/2D/ImguiManager.h"
#include "../../Engine/base/Texture/TextureManager.h"

void Course::Initialize(LevelData::MeshData* data)
{

	// メッシュオブジェクト
	MeshObject::Initialize(data);

	// コーステクスチャ
	const std::string kCourseTextureFileName = "Resources/default/white2x2.png";
	courseTextureHandle_ = TextureManager::Load(kCourseTextureFileName, DirectXCommon::GetInstance());

}

void Course::Update()
{

	// メッシュオブジェクト
	MeshObject::Update();

}

void Course::ImGuiDraw()
{

	ImGui::Begin("Course");
	CoursePolygon polygon = {};
	for (uint32_t i = 0; i < coursePolygons_.size(); ++i) {
		polygon = coursePolygons_[i];
		ImGui::Text("%d個目", i);
		ImGui::Text("位置0 x:%7.2f y:%7.2f z:%7.2f", polygon.positions[0].x, polygon.positions[0].y, polygon.positions[0].z);
		ImGui::Text("位置1 x:%7.2f y:%7.2f z:%7.2f", polygon.positions[1].x, polygon.positions[1].y, polygon.positions[1].z);
		ImGui::Text("位置2 x:%7.2f y:%7.2f z:%7.2f", polygon.positions[2].x, polygon.positions[2].y, polygon.positions[2].z);
		ImGui::Text("法線 x:%7.2f y:%7.2f z:%7.2f", polygon.normal.x, polygon.normal.y, polygon.normal.z);
		ImGui::Separator();

	}

	ImGui::End();

}
