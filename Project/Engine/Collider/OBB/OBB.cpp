#include "OBB.h"

void OBB::Initialize(const Vector3& center, const Vector3& otientatuonsX, const Vector3& otientatuonsY, const Vector3& otientatuonsZ, const Vector3& size, ColliderParentObject parentObject)
{

	Collider::Initialize(parentObject);

	center_ = center;//中心
	otientatuons_[0] = otientatuonsX;//座標軸
	otientatuons_[1] = otientatuonsY;//座標軸
	otientatuons_[2] = otientatuonsZ;//座標軸
	size_ = size;//座標軸方向の長さの半分

}

void OBB::Initialize(const Vector3& center, const Matrix4x4& rotateMatrix, const Vector3& size, ColliderParentObject parentObject)
{

	Collider::Initialize(parentObject);

	center_ = center;//中心
	SetOtientatuons(rotateMatrix);
	size_ = size;//座標軸方向の長さの半分

}

void OBB::SetOtientatuons(const Vector3& otientatuonsX, const Vector3& otientatuonsY, const Vector3& otientatuonsZ)
{

	otientatuons_[0] = otientatuonsX;//座標軸
	otientatuons_[1] = otientatuonsY;//座標軸
	otientatuons_[2] = otientatuonsZ;//座標軸

}

void OBB::SetOtientatuons(const Matrix4x4& rotateMatrix)
{

	otientatuons_[0].x = rotateMatrix.m[0][0];
	otientatuons_[0].y = rotateMatrix.m[0][1];
	otientatuons_[0].z = rotateMatrix.m[0][2];

	otientatuons_[1].x = rotateMatrix.m[1][0];
	otientatuons_[1].y = rotateMatrix.m[1][1];
	otientatuons_[1].z = rotateMatrix.m[1][2];

	otientatuons_[2].x = rotateMatrix.m[2][0];
	otientatuons_[2].y = rotateMatrix.m[2][1];
	otientatuons_[2].z = rotateMatrix.m[2][2];

}

void OBB::worldTransformUpdate()
{

	worldTransform_.transform_.translate = center_;
	worldTransform_.transform_.scale = size_;
	worldTransform_.UpdateMatrix();

}
