#include "Course.h"
#include "CoursePolygonType.h"
#include "../../Engine/2D/ImguiManager.h"
#include "../../Engine/base/Texture/TextureManager.h"
#include "../../Engine/3D/Model/ModelManager.h"
#include "CourseLoader.h"

void Course::Initialize(LevelData::MeshData* data)
{
	ModelManager::GetInstance()->AppendModel(CourseLoader::LoadCourseFile(data->directoryPath, data->flieName, *(GetCoursePolygonsAdress())));
	// メッシュオブジェクト
	MeshObject::Initialize(data);

	// コーステクスチャ
	const std::string kCourseTextureFileName = "Resources/Course/course.png";
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
		ImGui::Text("位置0 x:%7.2f y:%7.2f z:%7.2f", polygon.position0.x, polygon.position0.y, polygon.position0.z);
		ImGui::Text("位置1 x:%7.2f y:%7.2f z:%7.2f", polygon.position1.x, polygon.position1.y, polygon.position1.z);
		ImGui::Text("位置2 x:%7.2f y:%7.2f z:%7.2f", polygon.position2.x, polygon.position2.y, polygon.position2.z);
		ImGui::Text("法線 x:%7.2f y:%7.2f z:%7.2f", polygon.normal.x, polygon.normal.y, polygon.normal.z);
		ImGui::Separator();

	}

	ImGui::End();

}
