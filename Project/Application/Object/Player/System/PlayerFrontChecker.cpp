#include "PlayerFrontChecker.h"
#include "../Player.h"

#include <numbers>

void PlayerFrontChecker::Update()
{
	// 前方ベクトルを回転行列に適応
	frontVector_ = Matrix4x4::TransformNormal(Vector3(0.0f, 0.0f, 1.0f), player_->GetWorldTransformAdress()->rotateMatrix_);
	// Y軸無効
	frontVector_.y = 0.0f;
}

bool PlayerFrontChecker::FrontCheck(const Vector3& direct)
{
	// 対象へのベクトル
	Vector3 newDirect = Vector3(direct.x, 0.0f, direct.z);
	const Vector3 targetDirect = Vector3::Normalize(newDirect);
	// 閾値
	const float threshold = 0.75f;
	// 内積チェック
	float dot = Vector3::Dot(Vector3::Normalize(frontVector_), targetDirect);

	// リザルト
	return (dot >= threshold) ? true : false;
}
