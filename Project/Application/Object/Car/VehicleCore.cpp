#include "VehicleCore.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Input/Input.h"

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
	worldTransform_.transform_.translate.z = 0.0f;

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

	// パーツ構築クラス
	constructionSystem_ = std::make_unique<VehicleConstructionSystem>();
	constructionSystem_->Initialize(this);

	// 運転クラス
	driveSystem_ = std::make_unique<DriveSystem>();
	driveSystem_->SetOwner(this);
	driveSystem_->Initialize();
}

void VehicleCore::Update()
{
	// 運転・移動処理
	driveSystem_->Update();
	// 接続管理
	constructionSystem_->Update();
	// 基底
	Car::IParts::Update();
}

void VehicleCore::ImGuiDrawParts()
{
	ImGui::SeparatorText(className_.c_str());
	if (ImGui::TreeNode("Status"))
	{
		status_.ImGuiDraw();

		int armor = constructionSystem_->GetStatus().armor;
		int tire = constructionSystem_->GetStatus().tire;
		int en = constructionSystem_->GetStatus().engine;
		ImGui::InputInt("ArmorC", &armor);
		ImGui::InputInt("TireC", &tire);
		ImGui::InputInt("EngineC", &en);
		int st = constructionSystem_->GetDirections()->backForward;
		ImGui::InputInt("Back", &st);
		st = constructionSystem_->GetDirections()->forward;
		ImGui::InputInt("Forward", &st);
		st = constructionSystem_->GetDirections()->left;
		ImGui::InputInt("left", &st);
		st = constructionSystem_->GetDirections()->right;
		ImGui::InputInt("right", &st);


		ImGui::TreePop();
	}
	ImGui::BeginChild("Tab", ImVec2(400, 300), true, ImGuiWindowFlags_None);
	// トランスフォームに移動
	ImGuiTransform(0.1f);

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
	ImGui::BeginChild("SystemBlock", ImVec2(300, 200), true);
	if (ImGui::BeginTabBar("System")) {
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

void VehicleCore::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}
