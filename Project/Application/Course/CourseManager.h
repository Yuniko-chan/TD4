#pragma once
#include "Course.h"
#include "CourseLoader.h"
#include "../Object/Manager/GameSceneObjectManager.h"
#include<utility>
#include "../Object/Car/PickupPoint/PickupPointLists.h"
#include "../Object/Factory/ObjectFactory.h"
#include "../ClearSceneObject/CourseTraversalSystem.h"
#include "CourseCollisionSystem.h"

static const size_t kCourseNum = 6;
static const size_t kCourseFileCount = 3;

static const float kCourseScale_ = 2.0f;

static const std::array<std::string, kCourseFileCount> kCourseNameList = { "course_A.course","course_B.course","dirt.course"};

static const float kCourseDiameter = 100.0f * kCourseScale_;


static const std::array<Vector3, kCourseNum> courseOffsets_ = { Vector3{0.0f,0.0f,0.0f},
	Vector3{0.0f,0.0f,1.0f},
	Vector3{-1.0f,0.0f,1.0f},
	Vector3{-1.0f,0.0f,0.0f},
	Vector3{-2.0f,0.0f,0.0f},
	Vector3{-2.0f,0.0f,1.0f}};

static const Vector3 kCourseGroupOffset_ = {-200.0f* kCourseScale_,0.0f,200.0f* kCourseScale_ };

//pickup
static const std::array<Vector3, kPickupPointCount_> kPickupPointOffset = {
	Vector3{10.0f,-0.1f,15.0f},
	Vector3{-15.0f,-0.1f,0.0f},
	Vector3{15.0f,-0.1f,-10.0f}
};

static const std::array<std::string, kPickupPointCount_> kPickupPointDirectlyList = { "Engine","Tire","Frame" };
static const std::array<std::string, kPickupPointCount_> kPickupPointFileList = { "engine.obj","tire.obj","frame.obj" };

static const float kAddCourseBorder = 200;

static const size_t kWallVerticesNum = 36*3;
static const size_t kWallVerticesNum2 = 34*3;

//時計回りの面を表とする

static const float kCustomAreaWallWidth = 2.0f;

static const std::array<Vector3, kWallVerticesNum> kWallOffset{
	//右側面
	Vector3{kCourseDiameter / 2.0f,0.0f,-kCourseDiameter / 2},
	Vector3{kCourseDiameter / 2.0f,0.0f,kCourseDiameter +kCourseDiameter / 2},
	Vector3{kCourseDiameter / 2.0f,50.0f,-kCourseDiameter / 2},

	Vector3{kCourseDiameter / 2.0f,50.0f,-kCourseDiameter / 2},
	Vector3{kCourseDiameter / 2.0f,0.0f,kCourseDiameter + kCourseDiameter / 2},
	Vector3{kCourseDiameter / 2.0f,50.0f,kCourseDiameter + kCourseDiameter / 2},

	//左側面
	Vector3{-kCourseDiameter * 2.5f,0.0f,-kCourseDiameter / 2},
	Vector3{-kCourseDiameter * 2.5f,50.0f,-kCourseDiameter / 2},
	Vector3{-kCourseDiameter * 2.5f,0.0f,kCourseDiameter + kCourseDiameter / 2},

	Vector3{-kCourseDiameter * 2.5f,50.0f,-kCourseDiameter / 2},
	Vector3{-kCourseDiameter * 2.5f,50.0f,kCourseDiameter + kCourseDiameter / 2},
	Vector3{-kCourseDiameter * 2.5f,0.0f,kCourseDiameter + kCourseDiameter / 2},


	//奥面 左
	Vector3{-kCourseDiameter * 2.5f,0.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f,50.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) ,0.0f,kCourseDiameter * 1.5f},

	Vector3{-kCourseDiameter * 2.5f,50.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter/2.0f - 22.5f * kCourseScale_)  ,50.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_)  , 0.0f,kCourseDiameter * 1.5f},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_), 0.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_),50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_), 0.0f,kCourseDiameter * 1.5f},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) + (kCustomAreaWallWidth *kCourseScale_),50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_),50.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_), 0.0f,kCourseDiameter * 1.5f},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) , 0.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) , 0.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) ,50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) ,50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) , 0.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) ,50.0f,kCourseDiameter * 1.5f},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) , 0.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) , 50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_),0.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) , 50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_),50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f - 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_),0.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},


	//奥 右
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_),0.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_),50.0f,kCourseDiameter * 1.5f},
	Vector3{kCourseDiameter * 0.5f,0.0f,kCourseDiameter * 1.5f},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_),50.0f,kCourseDiameter * 1.5f},
	Vector3{kCourseDiameter * 0.5f,50.0f,kCourseDiameter * 1.5f},
	Vector3{kCourseDiameter * 0.5f, 0.0f,kCourseDiameter * 1.5f},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) , 0.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) ,50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) , 0.0f,kCourseDiameter * 1.5f},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) ,50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) ,50.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) , 0.0f,kCourseDiameter * 1.5f},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 0.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 0.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_),50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_),50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 0.0f,kCourseDiameter * 1.5f},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_),50.0f,kCourseDiameter * 1.5f},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 0.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) ,0.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},

	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) ,50.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},
	Vector3{-kCourseDiameter * 2.5f + (kCourseDiameter / 2.0f + 22.5f * kCourseScale_) ,0.0f,kCourseDiameter * 1.5f - (22.5f * kCourseScale_)},


	//手前 左
	Vector3{-kCourseDiameter * 2.5f,0.0f,-kCourseDiameter * 0.5f},
	Vector3{-kCourseDiameter * 0.5f ,0.0f,-kCourseDiameter * 0.5f},
	Vector3{-kCourseDiameter * 2.5f,50.0f,-kCourseDiameter * 0.5f},

	Vector3{-kCourseDiameter * 2.5f,50.0f,-kCourseDiameter * 0.5f},
	Vector3{-kCourseDiameter * 0.5f  , 0.0f,-kCourseDiameter * 0.5f},
	Vector3{-kCourseDiameter * 0.5f  ,50.0f,-kCourseDiameter * 0.5f},

	//
	Vector3{ (- 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f},
	Vector3{ (- 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_),50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_)},
	Vector3{ (-22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_)},

	Vector3{ (- 22.5f * kCourseScale_) + (kCustomAreaWallWidth *kCourseScale_),50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_)},
	Vector3{ (-22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_),50.0f,-kCourseDiameter * 0.5f },
	Vector3{ (- 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f},

	Vector3{ (- 22.5f * kCourseScale_) , 0.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_)},
	Vector3{ (- 22.5f * kCourseScale_) ,50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_)},
	Vector3{ (- 22.5f * kCourseScale_) , 0.0f,-kCourseDiameter * 0.5f },

	Vector3{ (- 22.5f * kCourseScale_) ,50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_)},
	Vector3{ (- 22.5f * kCourseScale_) ,50.0f,-kCourseDiameter * 0.5f},
	Vector3{ (- 22.5f * kCourseScale_) , 0.0f,-kCourseDiameter * 0.5f },

	Vector3{ (- 22.5f * kCourseScale_) , 0.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_)},
	Vector3{ (- 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_)},
	Vector3{ (- 22.5f * kCourseScale_) ,                                        50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },

	Vector3{ (- 22.5f * kCourseScale_) , 50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_)},
	Vector3{ (- 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_),0.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_)},
	Vector3{ (- 22.5f * kCourseScale_) + (kCustomAreaWallWidth * kCourseScale_),50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },



	//手前　右
	Vector3{ (22.5f * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f },
	Vector3{ (22.5f * kCourseScale_),50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },
	Vector3{ (22.5f * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },

	Vector3{ (22.5f * kCourseScale_),50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },
	Vector3{ (22.5f * kCourseScale_),50.0f,-kCourseDiameter * 0.5f },
	Vector3{ (22.5f * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f },

	Vector3{ (22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },
	Vector3{ (22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_),50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },
	Vector3{ (22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f },

	Vector3{ (22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_),50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },
	Vector3{ (22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_),50.0f,-kCourseDiameter * 0.5f },
	Vector3{ (22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f },

	Vector3{ (22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },
	Vector3{ (22.5f * kCourseScale_), 0.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },
	Vector3{ (22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_),                                        50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },

	Vector3{ (22.5f * kCourseScale_) - (kCustomAreaWallWidth * kCourseScale_), 50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },
	Vector3{ (22.5f * kCourseScale_),0.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },
	Vector3{ (22.5f * kCourseScale_),50.0f,-kCourseDiameter * 0.5f + (22.5f * kCourseScale_) },
};

static const std::array<Vector3, kWallVerticesNum> kWallNormals{
	//右
	Vector3{-1.0f,0.0f,0.0f},
	Vector3{-1.0f,0.0f,0.0f},
	Vector3{-1.0f,0.0f,0.0f},

	Vector3{-1.0f,0.0f,0.0f},
	Vector3{-1.0f,0.0f,0.0f},
	Vector3{-1.0f,0.0f,0.0f},

	//左
	Vector3{1.0f,0.0f,0.0f},
	Vector3{1.0f,0.0f,0.0f},
	Vector3{1.0f,0.0f,0.0f},

	Vector3{1.0f,0.0f,0.0f},
	Vector3{1.0f,0.0f,0.0f},
	Vector3{1.0f,0.0f,0.0f},

	//奥 左
	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},

	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},

	Vector3{1.0f,0.0f,0.0f},
	Vector3{1.0f,0.0f,0.0f},
	Vector3{1.0f,0.0f,0.0f},

	Vector3{1.0f,0.0f,0.0f},
	Vector3{1.0f,0.0f,0.0f},
	Vector3{1.0f,0.0f,0.0f},

	Vector3{-1.0f,0.0f,0.0f},
	Vector3{-1.0f,0.0f,0.0f},
	Vector3{-1.0f,0.0f,0.0f},

	Vector3{-1.0f,0.0f,0.0f},
	Vector3{-1.0f,0.0f,0.0f},
	Vector3{-1.0f,0.0f,0.0f},

	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},

	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},

	//奥 右
	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},

	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},
	Vector3{0.0f,0.0f,-1.0f},
};

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

	//コース踏破数を送る
	void AdaptCourseTraversalNum() { CourseTraversalSystem::SetCourseTraversalNum(courseTraversalNum_); };

	//壁生成
	void CreateWall();

	void SetCourseCollisionSystem(CourseCollisionSystem* courseCollisionSystem) { courseCollisionSystem_ = courseCollisionSystem; };

	int32_t* GetCourseTraversalNumAdrres() { return &courseTraversalNum_; };

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

	//コース踏破数
	int32_t courseTraversalNum_ = 0;

	CourseCollisionSystem* courseCollisionSystem_ = nullptr;
};
