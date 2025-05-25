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

	globalVariables->AddItem(groupName, "inVehicleOffset", inVehicleOffset_);
	globalVariables->AddItem(groupName, "inVehicleRotation", inVehicleRotation_);

	globalVariables->AddItem(groupName, "onFootRotation", onFootRotation_);
	globalVariables->AddItem(groupName, "onFootOffset", onFootOffset_);

	ApplyGlobalVariables();

	offset_ = Vector3(0.0f, offsetHeight_, offsetLength_);
	usedDirection_ = true;

	ChangeRequest(AngleMode::kPlayer, float(30.0f));
}

void FollowCamera::Update(float elapsedTime) {

#ifdef _DEMO
	ApplyGlobalVariables();
#endif // _DEMO
	

	if (modeRequest_) {
		// モード設定
		mode_ = modeRequest_.value();
		switch (mode_)
		{
		case FollowCamera::AngleMode::kPlayer:
			startPoint_ = Vector3(inVehicleOffset_);
			endPoint_ = Vector3(onFootOffset_);
			startDirection_ = Vector3(inVehicleRotation_);
			endDirection_ = Vector3(onFootRotation_);
			break;
		case FollowCamera::AngleMode::kVehicle:
			startPoint_ = Vector3(onFootOffset_);
			endPoint_ = Vector3(inVehicleOffset_);
			startDirection_ = Vector3(onFootRotation_);
			endDirection_ = Vector3(inVehicleRotation_);
			break;
		default:
			break;
		}
		modeRequest_ = std::nullopt;
	}

	// 遷移
	if (transitionTimer_.IsActive()) {
		offset_ = Ease::Easing(Ease::EaseName::Lerp, startPoint_, endPoint_, transitionTimer_.GetElapsedFrame());
		rotateDirection_ = Ease::Easing(Ease::EaseName::Lerp, startDirection_, endDirection_, transitionTimer_.GetElapsedFrame());
		if (transitionTimer_.IsEnd()) {
			transitionTimer_.End();
		}
	}
	// タイマー更新
	transitionTimer_.Update();

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
	ImGui::Begin("FollowCamera");
	ImGui::DragFloat3("Position", &transform_.translate.x);
	ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Offset", &offset_.x, 0.01f);
	ImGui::DragFloat3("RotateVector", &rotateDirection_.x, 0.01f);
	ImGui::Checkbox("UseDirection", &usedDirection_);
	if (ImGui::Button("Change")) {
		if (mode_ == AngleMode::kPlayer) {
			modeRequest_ = AngleMode::kVehicle;
		}
		else {
			modeRequest_ = AngleMode::kPlayer;
		}
	}
	ImGui::End();
}

void FollowCamera::ChangeRequest(AngleMode mode, float frame)
{
	modeRequest_ = mode;
	transitionTimer_.Start(frame);
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
			// 自分の回転
			Matrix4x4 from = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, Vector3::Normalize(rotateDirection_));
			// 対象の回転
			Matrix4x4 to = target_->rotateMatrix_;
			return Matrix4x4::Multiply(from,to);
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

	inVehicleOffset_ = globalVariables->GetVector3Value(groupName, "inVehicleOffset");
	inVehicleRotation_ = globalVariables->GetVector3Value(groupName, "inVehicleRotation");

	onFootOffset_ = globalVariables->GetVector3Value(groupName, "onFootOffset");
	onFootRotation_ = globalVariables->GetVector3Value(groupName, "onFootRotation");
}
