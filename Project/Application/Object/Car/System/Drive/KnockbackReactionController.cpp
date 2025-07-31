#include "KnockbackReactionController.h"
#include "../../VehicleCore.h"

#include "../../../GameTimer/GameTimeSystem.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void KnockbackReactionController::Update() 
{
	slowTimer_.Update(1.0f / GameTimeSystem::GetInstance()->GetTimeScale());

	if (acceptCount_ != 0)
	{
		acceptCount_ = 0;
		// 押し出し向き
		pushDirection_ = Vector3::Normalize(Vector3(totalPower_.x, 0.0f, totalPower_.z));

		// 向きの変更
		BeginDirectionAdjustment();

	}
	if (slowTimer_.IsActive()) {
		UpdateDirectionAdjustment();
	}

	// スロータイムの終了処理
	if (slowTimer_.IsEnd())
	{
		// 向き変更終了処理
		EndDirectionAdjustment();
	}

}

void KnockbackReactionController::BeginDirectionAdjustment()
{
	// スローでなければ一度処理を入れる
	if (!slowTimer_.IsActive()) {
		slowTimer_.Start(1.0f);
		const float kRotateSlowScale = 0.1f;
		GameTimeSystem::GetInstance()->SetTimeScale(kRotateSlowScale);
	}

}

void KnockbackReactionController::EndDirectionAdjustment()
{
	// タイムスケール戻す
	GameTimeSystem::GetInstance()->SetTimeScale(1.0f);
	// 押し出し
	Vector3 push = Vector3::Normalize(totalPower_);
	push.y = 0.0f;
	knockback_ += push * Vector3::Length(totalPower_);

	totalPower_ = {};
	pushDirection_ = {};
}

void KnockbackReactionController::UpdateDirectionAdjustment()
{
	Vector3 ownerDirection = owner_->GetWorldTransformAdress()->direction_;
	float dotAngle = 0.5f;
	if (Vector3::Dot(ownerDirection, pushDirection_) >= dotAngle) {
		Matrix4x4 fromDirectionRotateMatrix = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, pushDirection_);

		owner_->posture_ = fromDirectionRotateMatrix/* * owner_->posture_*/;
		owner_->GetWorldTransformAdress()->direction_ = Matrix4x4::TransformNormal(pushDirection_, owner_->posture_);
	}
}

void KnockbackReactionController::OnEngineBroken(const Vector3& direction)
{
	float powerFactor = 150.0f;
	Vector3 power = direction;
	power *= powerFactor;
	// パワー向き
	Vector3 powerDirection = Vector3(direction.x, 0, direction.z);
	// 車体向き
	Vector3 vehicleDirection = Vector3(owner_->GetWorldTransformAdress()->direction_.x, 0, owner_->GetWorldTransformAdress()->direction_.z);

	// 内積によって与える力を決めます
	float directDot = Vector3::Dot(powerDirection, vehicleDirection);
	// 後寄り
	Vector3 addPower = power;
	if (directDot <= -0.5f) {
		addPower *= (1.0f / 16.0f);
	}
	else if (directDot <= -0.25f) {
		addPower *= (1.0f / 8.0f);
	}
	else if (directDot <= 0.35f) {
		addPower *= (1.0f / 4.0f);
	}
	else if (directDot <= 0.65f) {
		addPower *= (1.0f / 2.0f);
	}
	else {
		addPower *= (1.0f / 1.0f);
	}
	// 合計のパワー
	totalPower_ += addPower;

	//float length = Vector3::Length(power);
	// 入力カウント
	acceptCount_++;
	//velocity_ += Vector3(0.0f, 0.0f, 1.0f) * length;
	//if (!isPush_) {
	//	pushCount_++;
	//}
	//pushPower_ += Vector3(0.0f, 0.0f, 1.0f) * length;
	//pushVector_.second += power;

	//direction;
}

Vector3 KnockbackReactionController::Execute()
{
	Vector3 result = owner_->GetWorldTransformAdress()->transform_.translate;
	if (knockback_ != Vector3(0.0f, 0.0f, 0.0f)) {
		
		float knockBackDecayFactor = 0.15f;
		// ノックバックパワー計算
		knockback_ = Ease::Easing(Ease::EaseName::Lerp, knockback_, Vector3(0, 0, 0), knockBackDecayFactor);
		// 座標計算
		result += knockback_ * GameTimeSystem::GetInstance()->GetDeltaTime();
	}
	return result;
}