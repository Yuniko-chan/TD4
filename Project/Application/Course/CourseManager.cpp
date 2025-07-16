#include "CourseManager.h"
#include "CourseLoader.h"
#include "../../Engine/3D/Model/ModelManager.h"
#include "../../Engine/Math/Vector/Vector3.h"
#include "../../Engine/Math/RandomEngine.h"
#include "../Object/CustomArea/CustomArea.h"
#include "../Object/Factory/ObjectCreate.h"

void CourseManager::Initialize(GameSceneObjectManager* objectManager, LevelDataManager* levelDataManager) {
	courseIndex_ = 0;
	objectManager_ = objectManager;
	levelDataManager_ = levelDataManager;
	objectFactory_ = objectManager_->GetObjectFactory();
	//全コース用データをロード
	for (size_t i = 0; i < kCourseFileCount;i++) {
		CourseLoader::LoadCourseFileFromManager("Resources/Course",kCourseNameList[i], courseDatas_[i]);
		ModelManager::GetInstance()->AppendModel(CourseLoader::CreateCourseModel(courseDatas_[i], kCourseNameList[i]));
	}

	//仮でグループ設定
	nowGroup_ = 0;
	createdGroup_ = 0;
	courseList_.emplace_back(std::array<Course*, kCourseNum>{});

	// 今追加した要素の参照を得る
	//std::array<Course*, kCourseNum>&group = (courseList_.back());

	//ランダムに配置する処理
	PlaceCourseRandom();
	
	nowGroup_++;
	createdGroup_++;
	
	//仮二個目
	/*for (int a = 0; a < 10; a++) {
		nowGroup_++;
		for (size_t i = 0; i < kCourseNum; i++) {
			isPlaced_[i] = false;
		}
		courseList_.emplace_back(std::array<Course*, kCourseNum>{});
		PlaceCourseRandom();
	}*/
}

void CourseManager::Update() {
	//条件は仮
	if (player_->GetWorldTransformAdress()->GetWorldPosition().z >= float(nowGroup_-1) * kAddCourseBorder) {
		//addCourseToGameScene_();
	}
}

void CourseManager::AddCourse() {
	addCourseToGameScene_();
}

void CourseManager::CreateCourse(const std::string& fileName, CourseImportData* courseInportData,const Vector3& offset, int rotate, size_t courseNum) {
	LevelData::MeshData objectData;
	EulerTransform transform;

	//グループでのオフセット計算
	static Vector3 groupOffset = {0,0,0};
	groupOffset.x = kCourseGroupOffset_.x* nowGroup_;
	groupOffset.z = kCourseGroupOffset_.z* nowGroup_;

	transform.rotate = { 0,0,0 };
	transform.scale = { 1.0f,1.0f,1.0f };
	transform.translate.x = offset.x * kCourseDiameter + groupOffset.x;
	transform.translate.y = offset.y * kCourseDiameter;
	transform.translate.z = offset.z * kCourseDiameter + groupOffset.z;
	transform.rotate.y = float(rotate) * 3.141592f*0.5f;


	objectData.name = std::format("Course{}",courseIndex_);

	objectData.className = "Course";
	objectData.flieName = fileName;
	objectData.directoryPath = "Resources/Course";
	objectData.transform = transform;
	Course* object = new Course();
	object->Initialize(&objectData,courseInportData);
	objectManager_->AddObject(object);
	courseList_[nowGroup_][courseIndex_ % 6] = object;
	courseIndex_++;

	LevelData* levelData = levelDataManager_->GetLevelDatas(LevelIndex(kLevelIndexCourseA + courseNum));
	// レベルデータのオブジェクトを走査
	for (std::vector<LevelData::ObjectData>::iterator it = levelData->objectsData_.begin(); it != levelData->objectsData_.end(); ++it) {
		// オブジェクトの参照
		LevelData::ObjectData gimmickData = *it;
		if (std::holds_alternative<LevelData::GimmickData>(gimmickData)) {

			std::get<LevelData::GimmickData>(gimmickData).meshData.parentName = objectData.name;
			std::get<LevelData::GimmickData>(gimmickData).meshData.name = std::get<LevelData::GimmickData>(gimmickData).meshData.name + std::to_string(courseIndex_);
			// 型にあわせてInitialize
			IObject* gimmick;
			gimmick = objectFactory_->CreateObjectPattern(gimmickData);
			gimmick->GetWorldTransformAdress()->parent_ = object->GetWorldTransformAdress();
			gimmick->GetWorldTransformAdress()->UpdateMatrix();
			if (gimmick) {
				// listへ
				objectManager_->AddObject(gimmick);
				(gimmickList_.get())->push_back(gimmick);
			}
		}
	}
}

void CourseManager::PlaceCourseRandom() {

	//カスタムエリア
	CreateCustomizeArea(nowGroup_);

	//0
	Place0();


	//ダート埋め
	for (size_t i = 0; i < kCourseNum; i++) {
		if (!isPlaced_[i]) {
			CreateCourse(kCourseNameList[2], &courseDatas_[2], courseOffsets_[i],0,2);
		}
	}
}


int CourseManager::Place0() {
	int next = 0;
	isPlaced_[0] = true;
	if(RandomEngine::GetRandom(0, 1) > 0.5f) {
		CreateCourse(kCourseNameList[0], &courseDatas_[0], courseOffsets_[0],0,0);
		next = Place1(0);
	}
	else {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[0],0,1);
		next =Place3(0);
	}

	return next;
}

int CourseManager::Place1(int prev) {
	int next = 0;
	isPlaced_[1] = true;
	if (prev == 0) {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[1],0,1);
		next = Place2(1);
	}
	return next;
}

int CourseManager::Place2(int prev) {
	int next = 0;
	isPlaced_[2] = true;
	if (prev == 1) {
		if (RandomEngine::GetRandom(0, 1) > 0.5f) {
			CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[2],-1,1);
			next = Place3(2);
		}
		else {
			CreateCourse(kCourseNameList[0], &courseDatas_[0], courseOffsets_[2],-1,0);
			next = Place5(2);
		}
	}
	else {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[2],0,1);
		next = Place5(2);
	}

	return next;
}

int CourseManager::Place3(int prev) {
	int next = 0;
	isPlaced_[3] = true;
	if (prev == 0) {
		if (RandomEngine::GetRandom(0, 1) > 0.5f) {
			CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[3],-2,1);
			next = Place2(3);
		}
		else {
			CreateCourse(kCourseNameList[0], &courseDatas_[0], courseOffsets_[3],1,0);
			next = Place4(3);
		}
	}
	else {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[3],1,1);
		next = Place4(3);
	}

	return next;
}

int CourseManager::Place4(int prev) {
	int next = 0;
	isPlaced_[4] = true;
	if (prev == 3) {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[4],2,1);
		next = Place5(4);
	}

	return next;
}

int CourseManager::Place5(int prev) {
	int next = 6;
	isPlaced_[5] = true;
	if (prev == 2) {
		CreateCourse(kCourseNameList[1], &courseDatas_[1], courseOffsets_[5],-2,1);
		
	}
	else {
		CreateCourse(kCourseNameList[0], &courseDatas_[0], courseOffsets_[5],0,0);
	}

	return next;
}

void CourseManager::CreateCustomizeArea(size_t group) {
	//-250
	//グループでのオフセット計算
	static Vector3 center = { 0,0,-250.0f /5.0f};
	static Vector3 offset;
	offset.x = kCourseGroupOffset_.x * group;
	offset.z = kCourseGroupOffset_.z * group + center.z;
	offset.y = 0.2f;

	LevelData::MeshData objectData;
	EulerTransform transform;

	transform.rotate = { 0,0,0 };
	transform.scale = { 1.0f,1.0f,1.0f };
	transform.translate.x = offset.x ;
	transform.translate.y = offset.y ;
	transform.translate.z = offset.z ;
	

	objectData.name = std::format("CustomArea{}", group);

	objectData.className = "CustomArea";
	objectData.flieName = "custom.obj";
	objectData.directoryPath = "Resources/Model/Custom";
	objectData.transform = transform;
	
	OBB obb;
	obb.Initialize(objectData.transform.translate, Matrix4x4::MakeIdentity4x4(), {20.0f,5.0f,20.0f}, static_cast<ParentNullObject*>(nullptr));
	objectData.collider = obb;
	CustomArea* object = new CustomArea();
	object->Initialize(&objectData);
	objectManager_->AddObject(object);

	object->SetCourseManager(this);
	//各ピックアップポイント
	for (size_t i = 0; i < kPickupPointCount_;i++) {
		CreatePickUpPoint(transform.translate,i,group);
	}
	if (gimmickList_) {
		object->SetGimmickList(gimmickList_);
	}
	gimmickList_.reset(new std::vector<IObject*>);
}

void CourseManager::CreatePickUpPoint(const Vector3& center,size_t num,size_t group) {
	LevelData::MeshData objectData;
	EulerTransform transform;

	transform.rotate = { 0,0,0 };
	transform.scale = { 1.0f,1.0f,1.0f };
	transform.translate.x = center.x + kPickupPointOffset[num].x;
	transform.translate.y = center.y + kPickupPointOffset[num].y;
	transform.translate.z = center.z + kPickupPointOffset[num].z;


	//std::string name = kRegisterPickupPointNames_[num];

	objectData.name = kRegisterPickupPointNames_[num] + std::format("{}", group);

	objectData.className = kRegisterPickupPointNames_[num];
	objectData.flieName = kPickupPointFileList[num];
	objectData.directoryPath = "Resources/Model/PickupPoint/" + kPickupPointDirectlyList[num];
	objectData.transform = transform;

	OBB obb;
	obb.Initialize(objectData.transform.translate, Matrix4x4::MakeIdentity4x4(), { 0,0,0 }, static_cast<ParentNullObject*>(nullptr));
	objectData.collider = obb;
	LevelData::ObjectData wrappedObjectData = objectData;
	IObject* object = nullptr;
	if (num == 0) {
		object = ObjectCreate::CreateObjectEnginePoint(wrappedObjectData);
	}
	else if(num == 1){
		object = ObjectCreate::CreateObjectTirePoint(wrappedObjectData);
	}
	else {
		object = ObjectCreate::CreateObjectArmorPoint(wrappedObjectData);
	}

	objectManager_->AddObject(object);
	objectManager_->RegisterPickupPoint(object,objectData.className,objectData.name);
}

size_t CourseManager::AddCourseGroup() {
	for (size_t i = 0; i < kCourseNum; i++) {
		isPlaced_[i] = false;
	}
	courseList_.emplace_back(std::array<Course*, kCourseNum>{});
	PlaceCourseRandom();
	return ++nowGroup_-1;
}

void CourseManager::SetAddCourseFunction(std::function<void(void)> function) {
	addCourseToGameScene_ = function;
}