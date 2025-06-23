#include "OverheadCamera.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Math.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../Object/GlobalParameterManager.h"

void OverheadCamera::Initialize()
{
	BaseCamera::Initialize();

	BaseCamera::Update();

	usedDirection_ = true;


	cameraTransform_ = &transform_;

	// Globalの取得更新
	ApplyGlobalVariable();
}

void OverheadCamera::Update(float elapsedTime)
{
#ifdef _DEMO

	ApplyGlobalVariable();

#endif // _DEMO

	// 遷移更新
	TransitionUpdate();

	//追従対象がいれば
	if (target_) {
		// 追従座標の補間(Z軸を取ってくる)
		//const Vector3 kTargetPositionEnd = { 0.0f, 0.0f, target_->worldMatrix_.m[3][2] };
		const Vector3 kTargetPositionEnd = { target_->worldMatrix_.m[3][0],target_->worldMatrix_.m[3][1] ,target_->worldMatrix_.m[3][2] };
		// 前後で追従レートの変化を付ける
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

void OverheadCamera::ImGuiDraw()
{
	ImGui::SeparatorText("Overhead");
	ImGui::DragFloat3("Position", &transform_.translate.x);
	ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Offset", &offset_.x, 0.01f);
	ImGui::DragFloat3("RotateVector", &rotateDirection_.x, 0.01f);
	ImGui::Checkbox("UseDirection", &usedDirection_);

}

void OverheadCamera::ApplyGlobalVariable()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";

	// 追従レート
	offsetMoveRate_ = globalVariables->GetFloatValue(groupName, "offsetMoveRate");
	// オフセット
	offset_ = GlobalVariables::GetInstance()->GetVector3Value(groupName, "onFootOffset");
	// 開始点
	from_.first = GlobalVariables::GetInstance()->GetVector3Value(groupName, "inVehicleOffset");
	from_.second = GlobalVariables::GetInstance()->GetVector3Value(groupName, "inVehicleRotation");
	// 終着点
	to_.first = GlobalVariables::GetInstance()->GetVector3Value(groupName, "onFootOffset");
	to_.second = GlobalVariables::GetInstance()->GetVector3Value(groupName, "onFootRotation");

}

void OverheadCamera::TransitionUpdate()
{
	TransitionCameraModule::TransitionUpdate();

	if (transitionTimer_.IsActive()) {
		rotateDirection_ = cameraDirection_;
	}

}

void OverheadCamera::SetTarget(const WorldTransform* target)
{
	this->target_ = target;
}

Matrix4x4 OverheadCamera::GetRotateMatrix()
{
	if (usedDirection_) {
		rotateDirection_ = Vector3::Normalize(rotateDirection_);
		return Matrix4x4::DirectionToDirection(Vector3::FrontVector(), rotateDirection_);
	}
	// 無ければデフォルト
	return Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);
}

Vector3 OverheadCamera::OffsetCalc()
{
	//追従対象からカメラまでのオフセット
	Vector3 offset = offset_;

	Matrix4x4 rotateMatrix = GetRotateMatrix();

	//オフセットをカメラの回転に合わせて回転させる
	offset = Matrix4x4::TransformNormal(offset, rotateMatrix);

	return offset;
}
