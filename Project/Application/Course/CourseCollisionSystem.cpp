#include "CourseCollisionSystem.h"
#include <algorithm>

// ブロードフェーズで使用する、距離判定
const float CourseCollisionSystem::kDistanceJudgment_ = 20.0f;
// ポリゴンエリアの原点
const Vector3 CourseCollisionSystem::kPolygonAreasOrigin_ = { -10.0f, -10.0f, -10.0f };
// ポリゴンエリアの長さ
const Vector3 CourseCollisionSystem::kPolygonAreasLength_ = { 20.0f, 20.0f, 20.0f };

void CourseCollisionSystem::Initialize()
{
}

void CourseCollisionSystem::Execute()
{
	
	// 登録分回す
	for (std::list<MeshObject*>::iterator itr = collidingObjects_.begin();
		itr != collidingObjects_.end();++itr) {
		// CPU側でOBBとの距離判定をとる->メッシュ登録
		DistanceJudgment(*itr);

		// ->GPU側で押し出し距離確認


	}

	// ->キック

	// 登録分回す
	for (std::list<MeshObject*>::iterator itr = collidingObjects_.begin();
		itr != collidingObjects_.end(); ++itr) {
	
		// ->CPU側で押し出し、回転（壁データはとらない）
		// ->OBB登録のオブジェクトのワールドトランスフォーム更新
	
	}

	// 後処理

	// 登録したオブジェクトリストをクリア
	collidingObjects_.clear();

}

void CourseCollisionSystem::ObjectRegistration(MeshObject* object)
{

	// オブジェクトリストに登録
	collidingObjects_.push_back(object);

}

void CourseCollisionSystem::SetCourse(Course* course)
{

	// コース
	course_ = course;

	// ポリゴンエリアに登録
	int x0 = 0, y0 = 0, z0 = 0;
	int x1 = 0, y1 = 0, z1 = 0;
	int x2 = 0, y2 = 0, z2 = 0;

	// 割る用の値
	Vector3 dividingValue = Vector3::Multiply(kPolygonAreasLength_, 1.0f / static_cast<float>(kPolygonAreasDiv_));

	// コースメッシュ分回す
	std::vector<CoursePolygon>* polygons = course_->GetCoursePolygonsAdress();
	for (uint32_t i = 0; i < polygons->size(); ++i) {

		// ポリゴン
		CoursePolygon polygon = (*polygons)[i];

		// 頂点位置、原点調整
		Vector3 vertex0 = polygon.positions[0] - kPolygonAreasOrigin_;
		Vector3 vertex1 = polygon.positions[1] - kPolygonAreasOrigin_;
		Vector3 vertex2 = polygon.positions[2] - kPolygonAreasOrigin_;

		// エリア番号
		x0 = static_cast<uint32_t>(vertex0.x / dividingValue.x);
		y0 = static_cast<uint32_t>(vertex0.y / dividingValue.y);
		z0 = static_cast<uint32_t>(vertex0.z / dividingValue.z);

		x1 = static_cast<uint32_t>(vertex1.x / dividingValue.x);
		y1 = static_cast<uint32_t>(vertex1.y / dividingValue.y);
		z1 = static_cast<uint32_t>(vertex1.z / dividingValue.z);

		x2 = static_cast<uint32_t>(vertex2.x / dividingValue.x);
		y2 = static_cast<uint32_t>(vertex2.y / dividingValue.y);
		z2 = static_cast<uint32_t>(vertex2.z / dividingValue.z);

		// 登録

		// 0番目
		polygonAreas[x0][y0][z0].push_back(polygon);

		// 1番目
		if (!(x0 == x1 && y0 == y1 && z0 == z1)) {
			polygonAreas[x1][y1][z1].push_back(polygon);
		}

		// 2番目
		if (!(x0 == x2 && y0 == y2 && z0 == z2) && !(x1 == x2 && y1 == y2 && z1 == z2)) {
			polygonAreas[x2][y2][z2].push_back(polygon);
		}

	}

}

void CourseCollisionSystem::DistanceJudgment(MeshObject* object)
{

	// オブジェクト情報
	Vector3 objectPosition = { 0.0f,0.0f,0.0f };
	object;
	// オブジェクトデータ取得

	// コースメッシュ分回す
	std::vector<CoursePolygon>* polygons = course_->GetCoursePolygonsAdress();
	for (uint32_t i = 0; i < polygons->size(); ++i) {
		
		// 距離をはかる
		CoursePolygon pilygon = (*polygons)[i];

		// 距離が近ければメッシュ登録
		if ((std::min(Vector3::Length(objectPosition - pilygon.positions[0]), 
				std::min(Vector3::Length(objectPosition - pilygon.positions[1]),
					Vector3::Length(objectPosition - pilygon.positions[2]))) < kDistanceJudgment_)){



		}
			

	}


}
