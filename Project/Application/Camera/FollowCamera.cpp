#include "FollowCamera.h"
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Engine/Math/Matrix/Matrix4x4.h"
#include "../../../Engine/Input/input.h"
#include <algorithm>
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Math.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/2D/ImguiManager.h"


void FollowCamera::Initialize() {

	BaseCamera::Initialize();

	BaseCamera::Update();

	// オフセットの追従レート
	offsetMoveRate_ = 0.1f;
	// ターゲット位置
	interTarget_ = {0.0f,0.0f,0.0f};

	// 回転固定
	const float rotateX = 0.1f;
	transform_.rotate.x = rotateX;

	ApplyGlobalVariables();
	offset_ = Vector3(to_.first);
	usedDirection_ = true;

}

void FollowCamera::Update(float elapsedTime) {

#ifdef _DEMO
	ApplyGlobalVariables();
#endif // _DEMO
	
	// 遷移用の受付
	TransitionUpdate();

	//追従対象がいれば
	if (target_) {
		// 追従座標の補間(Z軸を取ってくる)
		//const Vector3 kTargetPositionEnd = { 0.0f, 0.0f, target_->worldMatrix_.m[3][2] };
		const Vector3 kTargetPositionEnd = { target_->worldMatrix_.m[3][0],target_->worldMatrix_.m[3][1] ,target_->worldMatrix_.m[3][2] };
		//// 前後で追従レートの変化を付ける
		//if (interTarget_.z > kTargetPositionEnd.z) {
		//	interTarget_ = Ease::Easing(Ease::EaseName::Lerp, interTarget_, kTargetPositionEnd, 0.9f);
		//}
		//else {
		//	interTarget_ = Ease::Easing(Ease::EaseName::Lerp, interTarget_, kTargetPositionEnd, offsetMoveRate_);
		//}
		interTarget_ = Ease::Easing(Ease::EaseName::Lerp, interTarget_, kTargetPositionEnd, offsetMoveRate_);

		// オフセット
		Vector3 offset = OffsetCalc();

		transform_.translate = Vector3::Add(interTarget_, offset);

	}

	// ズーム
	Zoom(elapsedTime);

	// シェイク
	if (isShake_) {
		ShakeUpdate(elapsedTime);
	}

	// 正規化
	rotateDirection_ = Vector3::Normalize(rotateDirection_);
	// マッピング
	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(transform_.scale);
	Matrix4x4 rotateMatrix = GetRotateMatrix();
	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translate + shakeAddPosition_);

	transformMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix, translateMatrix));
	viewMatrix_ = Matrix4x4::Inverse(transformMatrix_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

	viewProjectionMatrix_->matrix = Matrix4x4::Multiply(viewMatrix_, projectionMatrix_);

	worldPositionMap_->worldPosition = { transformMatrix_.m[3][0],transformMatrix_.m[3][1], transformMatrix_.m[3][2] };


}

void FollowCamera::ImGuiDraw()
{
	ImGui::SeparatorText("FollowCamera");
	ImGui::DragFloat3("Position", &transform_.translate.x);
	ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Offset", &offset_.x, 0.01f);
	ImGui::DragFloat3("RotateVector", &rotateDirection_.x, 0.01f);
	ImGui::DragFloat("OffsetPlus", &zoomOutOffset_);
	ImGui::Checkbox("UseDirection", &usedDirection_);
}

void FollowCamera::SetTarget(const WorldTransform* target)
{

	target_ = target;

}

Matrix4x4 FollowCamera::GetRotateMatrix()
{
	// 対象がいる場合対象の回転行列を適応
	if (target_) {
		// 
		if (usedDirection_) {
			// カートがあれば、その向きに
			if (target_->parent_) {
				// 自分の回転
				Matrix4x4 from = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, Vector3::Normalize(rotateDirection_));
				// 対象の回転
				Matrix4x4 to = target_->parent_->rotateMatrix_;
				return Matrix4x4::Multiply(from, to);
			}
			//// 自分の回転
			//Matrix4x4 from = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, Vector3::Normalize(rotateDirection_));
			//// 対象の回転
			//Matrix4x4 to = target_->rotateMatrix_;
			//return Matrix4x4::Multiply(from,to);
			rotateDirection_ = Vector3::Normalize(rotateDirection_);
			return Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, this->rotateDirection_);

		}

		return Matrix4x4::Multiply(Matrix4x4::MakeRotateXYZMatrix(transform_.rotate), target_->rotateMatrix_);
	}
	if (usedDirection_) {
		// 回転行列作成
		// 正規化
		rotateDirection_ = Vector3::Normalize(rotateDirection_);
		return Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, this->rotateDirection_);
	}
	// 無ければデフォルト
	return Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);

}

Vector3 FollowCamera::OffsetCalc()
{

	//追従対象からカメラまでのオフセット
	Vector3 offset = offset_;
	offset.z -= zoomOutOffset_;

	Matrix4x4 rotateMatrix = GetRotateMatrix();

	//オフセットをカメラの回転に合わせて回転させる
	offset = Matrix4x4::TransformNormal(offset, rotateMatrix);

	return offset;

}

void FollowCamera::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "DriveCamera";
	// 移動
	offsetMoveRate_ = globalVariables->GetFloatValue(groupName, "TrackingDelay");
	// オフセット
	offset_ = globalVariables->GetVector3Value(groupName, "Position");
	// 終着点
	to_.first = globalVariables->GetVector3Value(groupName, "Position");
	to_.second = globalVariables->GetVector3Value(groupName, "RotateVector");

	// 開始点
	from_.first = globalVariables->GetVector3Value("OverheadCamera", "Position");
	from_.second = globalVariables->GetVector3Value("OverheadCamera", "RotateVector");
}

void FollowCamera::TransitionUpdate()
{
	TransitionCameraModule::TransitionUpdate();
	// 遷移中なら向きをモジュール側の値に
	if (transitionTimer_.IsActive()) {
		offset_ = currentPose_.first;
		rotateDirection_ = currentPose_.second;
	}
}
