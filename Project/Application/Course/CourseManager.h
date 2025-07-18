#pragma once
#include "Course.h"
#include "CourseLoader.h"
#include "../Object/Manager/GameSceneObjectManager.h"
#include<utility>
#include "../Object/Car/PickupPoint/PickupPointLists.h"
#include "../Object/Factory/ObjectFactory.h"

static const size_t kCourseNum = 6;
static const size_t kCourseFileCount = 3;

static const std::array<std::string, kCourseFileCount> kCourseNameList = { "course_A.course","course_B.course","dirt.course"};

static const float kCourseDiameter = 100.0f * 1.0f;


static const std::array<Vector3, kCourseNum> courseOffsets_ = { Vector3{0.0f,0.0f,0.0f},
	Vector3{0.0f,0.0f,1.0f},
	Vector3{-1.0f,0.0f,1.0f},
	Vector3{-1.0f,0.0f,0.0f},
	Vector3{-2.0f,0.0f,0.0f},
	Vector3{-2.0f,0.0f,1.0f}};

static const Vector3 kCourseGroupOffset_ = {-1000.0f/5.0f,0.0f,1000.0f/5.0f};

//pickup
static const std::array<Vector3, kPickupPointCount_> kPickupPointOffset = {
	Vector3{10.0f,-0.1f,15.0f},
	Vector3{-15.0f,-0.1f,0.0f},
	Vector3{15.0f,-0.1f,-10.0f}
};

static const std::array<std::string, kPickupPointCount_> kPickupPointDirectlyList = { "Engine","Tire","Frame" };
static const std::array<std::string, kPickupPointCount_> kPickupPointFileList = { "engine.obj","tire.obj","frame.obj" };

static const float kAddCourseBorder = 200;

class CourseManager
{
public:
	CourseManager() {};
	~CourseManager() {};

	void Initialize(GameSceneObjectManager* objectManager,LevelDataManager* levelDataManager);

	void Update();

	void AddCourse();

	std::array<Course*, kCourseNum>& GetCourseList() { return courseList_[0]; };
	std::array<Course*, kCourseNum>& GetCourseList(size_t group) { return courseList_[group]; };

	//コースグループ一個分増やす
	size_t AddCourseGroup();

	void SetAddCourseFunction(std::function<void(void)> function);
	void SetPlayer(MeshObject* object) { player_ = object; };

private:
	std::array<CourseImportData, kCourseFileCount> courseDatas_;
	GameSceneObjectManager* objectManager_;
	size_t courseIndex_ = 0;

	bool isPlaced_[kCourseNum] = { false };

	size_t nowGroup_;
	size_t createdGroup_;
	std::vector<std::array<Course*, kCourseNum>> courseList_;

	//ギミック削除用リストのポインタ
	std::unique_ptr<std::vector<IObject*>> gimmickList_ = nullptr;

	//std::array<std::function<int(int)>,kCourseNum> placeOnce_;

	//コースを一個生成
	void CreateCourse(const std::string& fileName, CourseImportData* courseInportData,const Vector3& offset,int rotate,size_t cousrseNum);

	//ランダムにコースを配置する
	void PlaceCourseRandom();

	int Place0();
	int Place1(int prev);
	int Place2(int prev);
	int Place3(int prev);
	int Place4(int prev);
	int Place5(int prev);

	//カスタマイズエリア生成
	void CreateCustomizeArea(size_t group);
	void CreatePickUpPoint(const Vector3& center, size_t num, size_t group);

	std::function<void(void)> addCourseToGameScene_;
	MeshObject* player_;

	// レベルデータマネージャー
	LevelDataManager* levelDataManager_;

	ObjectFactory* objectFactory_;
};
