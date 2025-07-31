#include "VehicleCore.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"

#include "../GameObjectsList.h"
#include "../../Collider/CollisionConfig.h"
#include "../Utility/Calc/TransformHelper.h"

VehicleCore::VehicleCore()
{
}

VehicleCore::~VehicleCore()
{
}

void VehicleCore::Initialize(LevelData::MeshData* data)
{
	// 基底
	MeshObject::Initialize(data);
	material_->SetEnableLighting(HalfLambert);
	// ベクトルで向きを決めるように
	worldTransform_.usedDirection_ = true;
	worldTransform_.transform_.translate = GlobalVariables::GetInstance()->GetVector3Value("Vehicle", "Core_InitPosition");
	worldTransform_.direction_ = {0.0f,0.0f,1.0f};
	worldTransform_.UpdateMatrix();
	posture_ = Matrix4x4::DirectionToDirection({0,0,1.0f},worldTransform_.direction_);
	atNormal_ = Matrix4x4::MakeIdentity4x4();
	worldTransform_.worldMatrix_ = Matrix4x4::MakeScaleMatrix(worldTransform_.transform_.scale) * posture_ * Matrix4x4::MakeTranslateMatrix(worldTransform_.transform_.translate);
	// 衝突マスク
	collisionAttribute_ = kCollisionAttributeVehicleParts_;
	collisionMask_ -= kCollisionAttributeVehicleParts_;

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	// アニメーション
	animation_ = std::make_unique<VehicleAnimation>();
	animation_->Initialize(model_);

	// ステータスクラス
	statusSystem_ = std::make_unique<VehicleStatus>();

	// パーツ構築クラス
	constructionSystem_ = std::make_unique<VehicleConstructionSystem>();
	constructionSystem_->SetOwner(this);
	constructionSystem_->Initialize();
	constructionSystem_->SetStatusManager(statusSystem_.get());

	// 運転クラス
	driveSystem_ = std::make_unique<DriveSystem>();
	driveSystem_->SetOwner(this);
	driveSystem_->SetStatusManager(statusSystem_.get());
	driveSystem_->Initialize();

	// オーバーヒート
	overheatSystem_ = std::make_unique<OverheatSystem>();
	overheatSystem_->SetOwner(this);
	overheatSystem_->SetMappingData(constructionSystem_->GetPartMappingPtr());
	
	// HP
	hpHandler_.SetOwner(this);
	hpHandler_.Setup();

	// コライダー更新
	ColliderUpdate();

	timeCountEngineSE_ = kTimeCountEngineSEMax_;
}

void VehicleCore::Update()
{
	// 接続管理
	constructionSystem_->Update();
	// 運転・移動処理
	driveSystem_->Update();
	// 
	overheatSystem_->Update();

	// アニメーション
	animation_->Update();
	// 基底
	// メッシュの更新
	MeshObject::Update();
	// 子専用更新（重力の適応）
	bool isParent = IsParent();
	if (!isParent) {
		// 仮の地面処理（後で消す）
		if (worldTransform_.GetWorldPosition().y <= 0.0f) {
			worldTransform_.transform_.translate.y = 0.0f;
		}
		worldTransform_.transform_.translate += Gravity::Execute();
	}
	// トランスフォームの更新
	worldTransform_.direction_ = Vector3::Normalize(worldTransform_.direction_);
	worldTransform_.UpdateMatrix();

	if (!isParent) {
		rotate_ = posture_ * atNormal_;
		worldTransform_.worldMatrix_ = Matrix4x4::MakeScaleMatrix(worldTransform_.transform_.scale) * posture_ * atNormal_ * Matrix4x4::MakeTranslateMatrix(worldTransform_.transform_.translate);
		worldTransform_.parentMatrix_ = Matrix4x4::MakeScaleMatrix(worldTransform_.transform_.scale) * posture_ * atNormal_ * Matrix4x4::MakeTranslateMatrix(worldTransform_.transform_.translate);
	}
	else {
		worldTransform_.UpdateMatrix(Matrix4x4::DirectionToDirection(Vector3(0, 0, 1), Vector3(0, 0, 1)));
	}
	// コライダーの更新
	ColliderUpdate();

	isDelete_ = false;

	EngineSERinging();

}

void VehicleCore::Draw(BaseCamera& camera)
{
	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = animation_->GetLocalMatrixManager();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);
}

void VehicleCore::ImGuiDrawParts()
{
	ImGui::SeparatorText(className_.c_str());
	ImGui::InputInt("道数", &roadCount_);
	ImGui::InputInt("ダート数", &dirtCount_);
	if (drivingLocation_ == CoursePolygonType::kCoursePolygonTypeRoad) {
		ImGui::Text("道");
	}
	else if (drivingLocation_ == CoursePolygonType::kCoursePolygonTypeDirt) {
		ImGui::Text("ダート");
	}
	else {
		ImGui::Text("おかしい");
	}

	if (ImGui::TreeNode("基本情報")) {
		Vector4 color = material_->GetColor();
		ImGui::DragFloat4("Color", &color.x, 0.01f);
		material_->SetColor(color);
		ImGui::BeginChild("Tab", ImVec2(400, 300), true, ImGuiWindowFlags_None);
		// トランスフォームに移動
		ImGuiTransform(0.1f);

		ImGui::Checkbox("IsDelete", &isDelete_);
		ImGui::EndChild();

		ImGui::TreePop();
	}
	// 試し
	if (ImGui::Button("押し出し")) {
		Vector3 direct = worldTransform_.direction_;
		worldTransform_.transform_.translate += direct * 10.0f;
	}


	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
	ImGui::BeginChild("SystemBlock", ImVec2(400, 350), true);
	if (ImGui::BeginTabBar("System")) {
		// エンジン
		if (ImGui::BeginTabItem("エンジン処理")) {
			this->driveSystem_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		// アニメーション
		if (ImGui::BeginTabItem("アニメーション")) {
			animation_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		// パーツ管理
		if (ImGui::BeginTabItem("パーツ管理")) {
			constructionSystem_->ImGuiDraw();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::PopStyleColor();
	ImGui::EndChild();	

	// ステータス
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
	ImGui::BeginChild("StatusBlock", ImVec2(400, 200), true);
	statusSystem_->ImGuiDraw();
	ImGui::EndChild();
	ImGui::PopStyleColor();



	//if (ImGui::Button("Release")) {
	//	pairPlayer_ = nullptr;
	//}

	ImGui::Text("\n");
}

void VehicleCore::EngineSERinging()
{

	if (isDrive_) {
		if (timeCountEngineSE_ <= kTimeCountEngineSEMax_) {
			timeCountEngineSE_ += kDeltaTime_;
		}
		else {
			audioManager_->PlayWave(kGameAudioNameIndexEngine);
			timeCountEngineSE_ = 0.0f;
		}
	}
	else {
		audioManager_->StopWave(kGameAudioNameIndexEngine);
		timeCountEngineSE_ = kTimeCountEngineSEMax_;
	}

}

void VehicleCore::SetAudioManager(GameAudioManager* audioManager)
{

	audioManager_ = audioManager;
	constructionSystem_->SetAudioManager(audioManager_);

}
