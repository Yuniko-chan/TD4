#include "CourseManager.h"
#include "CourseLoader.h"
#include "../../Engine/3D/Model/ModelManager.h"
#include "../../Engine/Math/Vector/Vector3.h"
void CourseManager::Initialize(GameSceneObjectManager* objectManager) {
	//初期化
	courseIndex_ = 0;
	objectManager_ = objectManager;
	//全コース用データをロード
	for (size_t i = 0; i < kCourseFileCount;i++) {
		CourseLoader::LoadCourseFileFromManager("Resources/Course",kCourseNameList[i], courseDatas_[i]);
		ModelManager::GetInstance()->AppendModel(CourseLoader::CreateCourseModel(courseDatas_[i]));
	}

	//オブジェクトマネージャーに登録(仮で同じものを六個作る)
	for (size_t i = 0; i < kCourseNum;i++) {
		CreateCourse(kCourseNameList[0], &courseDatas_[0], courseOffsets_[i]);
		courseIndex_++;
	}
	//
}

void CourseManager::CreateCourse(const std::string& fileName, CourseImportData* courseInportData,const Vector3& offset) {
	LevelData::MeshData objectData;
	EulerTransform transform;
	transform.rotate = { 0,0,0 };
	transform.scale = { 1.0f,1.0f,1.0f };
	transform.translate.x = offset.x * kCourseDiameter;
	transform.translate.y = offset.y * kCourseDiameter;
	transform.translate.z = offset.z * kCourseDiameter;


	objectData.name = std::format("Course{}",courseIndex_);

	objectData.className = "Course";
	objectData.flieName = fileName;
	objectData.directoryPath = "Resources/Course";
	objectData.transform = transform;
	Course* object = new Course();
	object->Initialize(&objectData,courseInportData);
	objectManager_->AddObject(object);
}