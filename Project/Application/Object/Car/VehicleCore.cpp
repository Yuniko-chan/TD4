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

	constructionSystem_ = std::make_unique<VehicleConstructionSystem>();
	constructionSystem_->Initialize(this);

	driveSystem_ = std::make_unique<DriveSystem>();
	driveSystem_->SetOwner(this);
	driveSystem_->SetTransform(&worldTransform_);

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
	static Vector3 angle = Vector3(worldTransform_.direction_);

	if (ImGui::Button("Rot")) {
		float rad = TransformHelper::CalculateXZVectorToRotateRadian(worldTransform_.direction_, Vector3(1.0f, 0.0f, 0.0f));
		angle = TransformHelper::XZRotateDirection(worldTransform_.direction_, rad);
	}
	ImGui::DragFloat3("Angle", &angle.x);

	// トランスフォームに移動
	ImGuiTransform(0.1f);
	if (ImGui::BeginTabBar("System")) {
		// ステート
		if (ImGui::BeginTabItem("Engine")) {
			this->driveSystem_->ImGuiDraw();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	if (ImGui::Button("Release")) {
		pairPlayer_ = nullptr;
	}

	if (ImGui::TreeNode("Childs")) {
		static float sAddValue = 2.0f;
		ImGui::DragFloat("StaticAddValue", &sAddValue, 0.01f);
		for (std::list<Car::IParts*>::iterator it = partsLists_.begin();
			it != partsLists_.end(); ++it) {
			ImGui::SeparatorText((*it)->GetName().c_str());
			std::string name = (*it)->GetName() + "Translate";
			ImGui::DragFloat3(name.c_str(), &(*it)->GetWorldTransformAdress()->transform_.translate.x, 0.1f);
			if (ImGui::Button("AddX")) {
				(*it)->GetWorldTransformAdress()->transform_.translate.x += sAddValue;
			}
			if (ImGui::Button("AddY")) {
				(*it)->GetWorldTransformAdress()->transform_.translate.y += sAddValue;
			}
			if (ImGui::Button("AddZ")) {
				(*it)->GetWorldTransformAdress()->transform_.translate.z += sAddValue;
			}
			ImGui::Text("\n");
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Status"))
	{
		status_.ImGuiDraw();
		ImGui::TreePop();
	}

	ImGui::Text("\n");
}

void VehicleCore::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
	colliderPartner, collisionData;
}
