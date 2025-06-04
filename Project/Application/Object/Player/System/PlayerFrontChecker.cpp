#include "PlayerFrontChecker.h"
#include "../Player.h"

#include <numbers>

void PlayerFrontChecker::Update()
{
	// 方向
	if (owner_->GetWorldTransformAdress()->usedDirection_) {
		frontVector_ = owner_->GetWorldTransformAdress()->direction_;
	}
	else {
		Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateYMatrix(owner_->GetWorldTransformAdress()->transform_.rotate.y);
		// 前方ベクトルを回転行列に適応
		frontVector_ = Matrix4x4::TransformNormal(Vector3(0.0f, 0.0f, 1.0f), rotateMatrix);
	}

	// Y軸無効
	frontVector_.y = 0.0f;
}

bool PlayerFrontChecker::FrontCheck(const Vector3& direct)
{
	// 対象へのベクトル
	Vector3 newDirect = Vector3(direct.x, 0.0f, direct.z);
	const Vector3 targetDirect = Vector3::Normalize(newDirect);
	// 閾値
	const float threshold = GlobalVariables::GetInstance()->GetFloatValue("Player","FrontThreshold");
	// 内積チェック
	float dot = Vector3::Dot(Vector3::Normalize(frontVector_), targetDirect);
	// リザルト
	return (dot >= threshold) ? true : false;
}

bool PlayerFrontChecker::IsInRange(const Vector3& target)
{
	// XZ平面上の位置
	Vector3 xzTarget = Vector3(target.x, 0.0f, target.z);
	Vector3 xzPlayer = Vector3(owner_->GetWorldTransformAdress()->GetWorldPosition().x, 0.0f, owner_->GetWorldTransformAdress()->GetWorldPosition().z);
	Vector3 distance = xzTarget - xzPlayer;
	// 距離の大きさ
	float distanceValue = float(std::sqrtf(std::powf(distance.x, 2) + std::powf(distance.y, 2) + std::powf(distance.z, 2)));
	// 最大距離
	const float maxRange = GlobalVariables::GetInstance()->GetFloatValue("Player", "FrontCatchRange");
	return (distanceValue <= maxRange) ? true : false;
}

bool PlayerFrontChecker::IsPointInRange(const Vector3& target)
{
	// XZ平面上の位置
	Vector3 xzTarget = Vector3(target.x, 0.0f, target.z);
	Vector3 xzPlayer = Vector3(owner_->GetWorldTransformAdress()->GetWorldPosition().x, 0.0f, owner_->GetWorldTransformAdress()->GetWorldPosition().z);
	Vector3 distance = xzTarget - xzPlayer;
	// 距離の大きさ
	float distanceValue = float(std::sqrtf(std::powf(distance.x, 2) + std::powf(distance.y, 2) + std::powf(distance.z, 2)));
	// 最大距離
	const float maxRange = GlobalVariables::GetInstance()->GetFloatValue("Player", "FrontCatchRange");
	return (distanceValue <= maxRange) ? true : false;
}
