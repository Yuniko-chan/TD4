#include "OverheadCamera.h"

void OverheadCamera::Initialize()
{
	BaseCamera::Initialize();

	BaseCamera::Update();

}

void OverheadCamera::Update(float elapsedTime)
{
	// ズーム
	Zoom(elapsedTime);

	// シェイク
	if (isShake_) {
		ShakeUpdate(elapsedTime);
	}
	//// 正規化
	//rotateDirection_ = Vector3::Normalize(rotateDirection_);
	//// マッピング
	//Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(transform_.scale);
	//Matrix4x4 rotateMatrix = GetRotateMatrix();
	//Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translate + shakeAddPosition_);

	//transformMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix, translateMatrix));
	//viewMatrix_ = Matrix4x4::Inverse(transformMatrix_);
	//projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

	//viewProjectionMatrix_->matrix = Matrix4x4::Multiply(viewMatrix_, projectionMatrix_);

	//worldPositionMap_->worldPosition = { transformMatrix_.m[3][0],transformMatrix_.m[3][1], transformMatrix_.m[3][2] };

}

Matrix4x4 OverheadCamera::GetRotateMatrix()
{
	return Matrix4x4();
}
