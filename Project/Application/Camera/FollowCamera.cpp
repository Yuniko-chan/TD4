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

	// オフセットの長さ
	offsetLength_ = 0.0f;
	// オフセットの高さ
	offsetHeight_ = 0.0f;
	// オフセットの追従レート
	offsetMoveRate_ = 0.1f;
	// ターゲット位置
	interTarget_ = {0.0f,0.0f,0.0f};

	// 回転固定
	const float rotateX = 0.1f;
	transform_.rotate.x = rotateX;

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "offsetLength", offsetLength_);
	globalVariables->AddItem(groupName, "offsetHeight", offsetHeight_);
	globalVariables->AddItem(groupName, "offsetMoveRate", offsetMoveRate_);

	globalVariables->AddItem(groupName, "inVehicleOffsetPosition", inVehicleOffsetPosition_);
	globalVariables->AddItem(groupName, "inVehicleRotate", inVehicleRotate_);

	ApplyGlobalVariables();

}

void FollowCamera::Update(float elapsedTime) {

#ifdef _DEMO
	ApplyGlobalVariables();
#endif // _DEMO


	//追従対象がいれば
	if (target_) {
		// 追従座標の補間(Z軸を取ってくる)
		//const Vector3 kTargetPositionEnd = { 0.0f, 0.0f, target_->worldMatrix_.m[3][2] };
		const Vector3 kTargetPositionEnd = { target_->worldMatrix_.m[3][0],target_->worldMatrix_.m[3][1] ,target_->worldMatrix_.m[3][2] };
		// 移動レート
		//const float kMoveRate = 0.1f;
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
	ImGui::Begin("FollowCamera");
	ImGui::DragFloat3("Position", &transform_.translate.x);
	ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);

	ImGui::End();
}

void FollowCamera::SetTarget(const WorldTransform* target)
{

	target_ = target;

}

Matrix4x4 FollowCamera::GetRotateMatrix()
{
	// 対象がいる場合対象の回転行列を適応
	if (target_) {
		// 対象に更に親がいる場合
		if (target_->parent_) {
			return Matrix4x4::Multiply(Matrix4x4::MakeRotateXYZMatrix(transform_.rotate), target_->rotateMatrix_);
		}
		return Matrix4x4::Multiply(Matrix4x4::MakeRotateXYZMatrix(transform_.rotate), target_->rotateMatrix_);
	}
	// 無ければデフォルト
	return Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);

}

Vector3 FollowCamera::OffsetCalc()
{

	//追従対象からカメラまでのオフセット
	Vector3 offset = { 0.0f, offsetHeight_, offsetLength_ };

	Matrix4x4 rotateMatrix = GetRotateMatrix();

	//オフセットをカメラの回転に合わせて回転させる
	offset = Matrix4x4::TransformNormal(offset, rotateMatrix);

	return offset;

}

void FollowCamera::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";

	offsetLength_ = globalVariables->GetFloatValue(groupName, "offsetLength");
	offsetHeight_ = globalVariables->GetFloatValue(groupName, "offsetHeight");
	offsetMoveRate_ = globalVariables->GetFloatValue(groupName, "offsetMoveRate");

	inVehicleOffsetPosition_ = globalVariables->GetVector3Value(groupName, "inVehicleOffsetPosition");
	inVehicleRotate_ = globalVariables->GetVector3Value(groupName, "inVehicleRotate");
}
