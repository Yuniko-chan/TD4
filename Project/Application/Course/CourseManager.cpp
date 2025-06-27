#include "CourseManager.h"
#include "CourseLoader.h"
#include "../../Engine/3D/Model/ModelManager.h"
#include "../../Engine/Math/Vector/Vector3.h"
#include "../../Engine/Math/RandomEngine.h"

void CourseManager::Initialize(GameSceneObjectManager* objectManager) {
	//初期化
	//placeOnce_[0] = Place0;

	courseIndex_ = 0;
	objectManager_ = objectManager;
	//全コース用データをロード
	for (size_t i = 0; i < kCourseFileCount;i++) {
		CourseLoader::LoadCourseFileFromManager("Resources/Course",kCourseNameList[i], courseDatas_[i]);
		ModelManager::GetInstance()->AppendModel(CourseLoader::CreateCourseModel(courseDatas_[i], kCourseNameList[i]));
	}

	//オブジェクトマネージャーに登録(仮で同じものを六個作る)
	/*for (size_t i = 0; i < kCourseNum; i++) {
		CreateCourse(kCourseNameList[i%2], &courseDatas_[i%2], courseOffsets_[i]);
	}*/
	//ランダムに配置する処理
	PlaceCourseRandom();
}

void CourseManager::CreateCourse(const std::string& fileName, CourseImportData* courseInportData,const Vector3& offset, int rotate) {
	LevelData::MeshData objectData;
	EulerTransform transform;
	transform.rotate = { 0,0,0 };
	transform.scale = { 5.0f,5.0f,5.0f };
	transform.translate.x = offset.x * kCourseDiameter;
	transform.translate.y = offset.y * kCourseDiameter;
	transform.translate.z = offset.z * kCourseDiameter;
	transform.rotate.y = float(rotate) * 3.141592f*0.5f;


	objectData.name = std::format("Course{}",courseIndex_);

	objectData.className = "Course";
	objectData.flieName = fileName;
	objectData.directoryPath = "Resources/Course";
	objectData.transform = transform;
	Course* object = new Course();
	object->Initialize(&objectData,courseInportData);
	objectManager_->AddObject(object);

	courseIndex_++;
}

void CourseManager::PlaceCourseRandom() {

	//0
	Place0();


	//ダート埋め
	for (size_t i = 0; i < kCourseNum; i++) {
		if (!isPlaced_[i]) {
			CreateCourse(kCourseNameList[0], &courseDatas_[0], courseOffsets_[i],0);
		}
	}
}


int CourseManager::Place0() {
	int next = 0;
	isPlaced_[0] = true;
	if(RandomEngine::GetRandom(0, 1) > 0.5f) {
		CreateCourse(kCourseNameList[0], &courseDatas_[0], courseOffsets_[0],0);
		next = Place1(0);
	}
	else {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[0],0);
		next =Place3(0);
	}

	return next;
}

int CourseManager::Place1(int prev) {
	int next = 0;
	isPlaced_[1] = true;
	if (prev == 0) {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[1],0);
		next = Place2(1);
	}
	return next;
}

int CourseManager::Place2(int prev) {
	int next = 0;
	isPlaced_[2] = true;
	if (prev == 1) {
		if (RandomEngine::GetRandom(0, 1) > 0.5f) {
			CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[2],-1);
			next = Place3(2);
		}
		else {
			CreateCourse(kCourseNameList[0], &courseDatas_[0], courseOffsets_[2],-1);
			next = Place5(2);
		}
	}
	else {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[2],0);
		next = Place5(2);
	}

	return next;
}

int CourseManager::Place3(int prev) {
	int next = 0;
	isPlaced_[3] = true;
	if (prev == 0) {
		if (RandomEngine::GetRandom(0, 1) > 0.5f) {
			CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[3],-2);
			next = Place2(3);
		}
		else {
			CreateCourse(kCourseNameList[0], &courseDatas_[0], courseOffsets_[3],1);
			next = Place4(3);
		}
	}
	else {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[3],1);
		next = Place4(3);
	}

	return next;
}

int CourseManager::Place4(int prev) {
	int next = 0;
	isPlaced_[4] = true;
	if (prev == 3) {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[4],2);
		next = Place5(4);
	}

	return next;
}

int CourseManager::Place5(int prev) {
	int next = 6;
	isPlaced_[5] = true;
	if (prev == 2) {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[5],-2);
		
	}
	else {
		CreateCourse(kCourseNameList[0], &courseDatas_[0], courseOffsets_[5],0);
	}

	return next;
}