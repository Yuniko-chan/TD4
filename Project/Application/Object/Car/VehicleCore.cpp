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
	worldTransform_.transform_.translate.y = 0.0f;
	worldTransform_.transform_.translate.z = 0.0f;
	worldTransform_.UpdateMatrix();
	
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
	hpHandler_.Initialize();

	// コライダー更新
	ColliderUpdate();
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
	// 仮の地面処理（後で消す）
	if (worldTransform_.GetWorldPosition().y <= 0.0f) {
		worldTransform_.transform_.translate.y = 0.0f;
		//return;
	}
	// トランスフォームの更新
	worldTransform_.direction_ = Vector3::Normalize(worldTransform_.direction_);
	worldTransform_.UpdateMatrix();
	// コライダーの更新
	ColliderUpdate();
	// 重力
	worldTransform_.transform_.translate += Gravity::Execute();

	isDelete_ = false;
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
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
	ImGui::BeginChild("SystemBlock", ImVec2(400, 200), true);
	if (ImGui::TreeNode("Status"))
	{
		// ステータス
		statusSystem_->ImGuiDraw();

		ImGui::TreePop();
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImGui::BeginChild("Tab", ImVec2(400, 300), true, ImGuiWindowFlags_None);
	// トランスフォームに移動
	ImGuiTransform(0.1f);

	ImGui::Checkbox("IsDelete", &isDelete_);

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
	ImGui::BeginChild("SystemBlock", ImVec2(300, 200), true);
	if (ImGui::BeginTabBar("System")) {
		if (ImGui::BeginTabItem("Animation")) {
			animation_->ImGuiDraw();
			ImGui::EndTabItem();
		}

		// ステート
		if (ImGui::BeginTabItem("Engine")) {
			this->driveSystem_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		// パーツ管理
		if (ImGui::BeginTabItem("パーツ管理")) {
			constructionSystem_->ImGuiDraw();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImGui::EndChild();


	if (ImGui::Button("Release")) {
		pairPlayer_ = nullptr;
	}

	ImGui::Text("\n");
}
