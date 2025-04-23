#include "Course.h"
#include "CoursePolygonType.h"
#include "../../Engine/2D/ImguiManager.h"

void Course::Initialize(LevelData::MeshData* data)
{

	// メッシュオブジェクト
	MeshObject::Initialize(data);

	// 三角ポリゴン（頂点三つ、法線、種類）
	/*
	// メッシュデータ取得
	Mesh* meshData = model_->GetMesh();
	// 頂点の数取得
	uint32_t vertexNum = meshData->GetSkinningInformationMap()->num;
	// 頂点のデータ
	VertexData* baseVertexData = meshData->GetVertMap();
	VertexData vertexDatas[3] = {};
	
	// 頂点の数 / 3分回す
	for (uint32_t i = 0; i < vertexNum / 3;++i)
	{
		vertexDatas[0] = *(baseVertexData + (i * 3));
		vertexDatas[1] = *(baseVertexData + (i * 3 + 1));
		vertexDatas[2] = *(baseVertexData + (i * 3 + 2));

		 // ポリゴン
		CoursePolygon polygon = {};
		polygon.positions[0] = { vertexDatas[0].position.x, vertexDatas[0].position.y, vertexDatas[0].position.z };
		polygon.positions[1] = { vertexDatas[1].position.x, vertexDatas[1].position.y, vertexDatas[1].position.z };
		polygon.positions[2] = { vertexDatas[2].position.x, vertexDatas[2].position.y, vertexDatas[2].position.z };
		polygon.normal = Vector3::Normalize(vertexDatas[0].normal + vertexDatas[1].normal + vertexDatas[2].normal);
		polygon.coursePolygonType = CoursePolygonType::kCoursePolygonTypeRoad;
		
		// 登録
		coursePolygons_.push_back(polygon);

	}*/

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
		ImGui::Text("%d個目, 種類番号:%d", i, polygon.coursePolygonType);
		ImGui::Text("位置0 x:%7.2f y:%7.2f z:%7.2f", polygon.positions[0].x, polygon.positions[0].y, polygon.positions[0].z);
		ImGui::Text("位置1 x:%7.2f y:%7.2f z:%7.2f", polygon.positions[1].x, polygon.positions[1].y, polygon.positions[1].z);
		ImGui::Text("位置2 x:%7.2f y:%7.2f z:%7.2f", polygon.positions[2].x, polygon.positions[2].y, polygon.positions[2].z);
		ImGui::Text("法線 x:%7.2f y:%7.2f z:%7.2f", polygon.normal.x, polygon.normal.y, polygon.normal.z);
		ImGui::Separator();

	}

	ImGui::End();

}
