#include "TireParts.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Utility/Calc/TransformHelper.h"
#include "../../../Engine/Math/Ease.h"

void TireParts::Initialize(LevelData::MeshData* data)
{
	Car::IParts::Initialize(data);

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void TireParts::Update()
{
	// 回転
	//Vector3 base = TransformHelper::YZRotateDirection(Vector3(0, 0, 1), spinRate_);
	//Vector3 rotateAxis = Vector3::Normalize(Vector3::Cross(base, steerDirection_));
	//float angle = Vector3::Dot(base, steerDirection_);
	//angle = std::clamp(angle, -1.0f, 1.0f);
	//angle = std::acosf(angle);

	//float spinTheta = spinRate_;

	// discard
	const float kDiscard = 0.001f;
	if (std::fabsf(spinRate_) < kDiscard) {
		spinRate_ = 0.0f;
	}

	Matrix4x4 yzRotate = Matrix4x4::MakeIdentity4x4();
	Matrix4x4 xzRotate = Matrix4x4::MakeIdentity4x4();

	if (spinRate_ != 0.0f) {
		// タイヤの長さ
		float spinRadius = 1.0f;
		// 回転量
		currentSpin_ += spinRate_ / spinRadius;
		//Vector3 next = TransformHelper::YZRotateDirection(steerDirection_, spinRate_);
		//worldTransform_.direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform_.direction_, next, 0.05f);

		// XZとYZの回転行列構築
		Vector3 yz = TransformHelper::YZRotateDirection(Vector3(0.0f, 0.0f, 1.0f), currentSpin_);
		xzRotate = Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), steerDirection_);
		yzRotate = Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), yz);
	}
	else {
		//worldTransform_.direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform_.direction_, steerDirection_, 0.05f);
		xzRotate = Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), steerDirection_);
	}
	//worldTransform_.direction_ = TransformHelper::YZRotateDirection(steerDirection_, spinRate_);

	Car::IParts::Update();

	// 回転行列専用で更新
	worldTransform_.UpdateMatrix(Matrix4x4::Multiply(yzRotate, xzRotate));
}

void TireParts::ImGuiDraw()
{

}

void TireParts::ImGuiDrawParts()
{
	ImGui::SeparatorText(name_.c_str());
	ImGuiDrawChildParts();

	ImGui::DragFloat3("Steer", &steerDirection_.x);
	ImGui::DragFloat("Spin", &spinRate_);
	ImGui::DragFloat3("Direction", &worldTransform_.direction_.x);

	static float moveValue = 0.1f;
	ImGui::DragFloat("SlideValue", &moveValue, 0.01f);
	// トランスフォーム用
	ImGuiTransform(moveValue);
	// コネクター
	if (connector_->IsParent()) {
		ImGui::Text("Parent");
	}
	ImGui::Text("\n");
}
