#pragma once
#include "Course.h"
#include "CourseLoader.h"
#include "../Object/Manager/GameSceneObjectManager.h"
#include<utility>


static const size_t kCourseNum = 6;
static const size_t kCourseFileCount = 1;

static const std::array<std::string, kCourseFileCount> kCourseNameList = {"demoCourse2.course"};

static const float kCourseDiameter = 100.0f;

static const std::array<Vector3, kCourseNum> courseOffsets_ = { Vector3{0.0f,0.0f,0.0f},
	Vector3{0.0f,0.0f,1.0f},
	Vector3{1.0f,0.0f,1.0f},
	Vector3{1.0f,0.0f,0.0f},
	Vector3{2.0f,0.0f,0.0f},
	Vector3{2.0f,0.0f,1.0f}};

class CourseManager
{
public:
	CourseManager() {};
	~CourseManager() {};

	void Initialize(GameSceneObjectManager* objectManager);

private:
	std::array<CourseImportData, kCourseFileCount> courseDatas_;
	GameSceneObjectManager* objectManager_;
	size_t courseIndex_ = 0;

	//コースを一個生成
	void CreateCourse(const std::string& fileName, CourseImportData* courseInportData,const Vector3& offset);
};
