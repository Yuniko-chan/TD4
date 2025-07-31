#include "DriveSystem.h"
#include "../../VehicleCore.h"
#include "../../CarLists.h"

#include "../../System/VehicleSystems.h"
#include "../../../GameTimer/GameTimeSystem.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

DriveSystem::DriveSystem()
{
	// エンジン
	driveEngine_ = std::make_unique<DriveEngine>();
	// ハンドル
	handling_ = std::make_unique<DriveHandling>();
	// リアクション
	knockbackReactionController_ = std::make_unique<KnockbackReactionController>();
}

void DriveSystem::Initialize()
{
	// ハンドルクラス
	handling_->SetOwner(owner_);

	// エンジングラス
	driveEngine_->SetOwner(owner_);

	// リアクション
	knockbackReactionController_->SetOwner(owner_);
	slowTimer_.End();
}

void DriveSystem::Update()
{
	knockbackReactionController_->Update();

	//slowTimer_.Update(1.0f / GameTimeSystem::GetInstance()->GetTimeScale());
	//if (pushCount_ != 0) {
	//	// 回転行列のやつから向きを取得

	//	pushVector_.second = Vector3::Normalize(totalDirection_);
	//	pushVector_.second.y = 0.0f;
	//	pushVector_.first = owner_->GetWorldTransformAdress()->direction_;
	//	pushVector_.first.y = 0.0f;

	//	isPush_ = false;
	//	pushCount_ = 0;
	//	// 
	//	if (!slowTimer_.IsActive()) {
	//		slowTimer_.Start(1.0f);
	//		const float kRotateSlowScale = 0.1f;
	//		GameTimeSystem::GetInstance()->SetTimeScale(kRotateSlowScale);
	//	}
	//}

	//if (slowTimer_.IsActive()) {
	//	if (Vector3::Dot(pushVector_.first, pushVector_.second) >= 0.5f) {
	//		Vector3 pushDirect = Vector3(totalDirection_.x, 0.0f, totalDirection_.z);
	//		pushDirect = Vector3::Normalize(pushDirect);
	//		Matrix4x4 fromDirectionRotateMatrix = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, pushDirect);

	//		owner_->posture_ = fromDirectionRotateMatrix/* * owner_->posture_*/;
	//		owner_->GetWorldTransformAdress()->direction_ = Matrix4x4::TransformNormal(pushDirect, owner_->posture_);
	//		//owner_->GetWorldTransformAdress()->direction_ = Vector3::Normalize(owner_->GetWorldTransformAdress()->direction_);
	//	}

	//}
	//if (slowTimer_.IsEnd()) {
	//	// タイムスケール戻す
	//	GameTimeSystem::GetInstance()->SetTimeScale(1.0f);
	//	// 押し出し
	//	Vector3 push = Vector3::Normalize(totalDirection_);
	//	push.y = 0.0f;
	//	knockBack_ += push * Vector3::Length(totalDirection_);
	//	
	//	pushPower_ = {};
	//	totalDirection_ = {};
	//}

	// オーバーヒートフラグ初期化
	status_->SetIsOverheat(false);

	//---それぞれのシステム処理---//
	// ハンドル処理
	handling_->PreUpdate();
	// エンジン処理
	driveEngine_->Update();
	// 速度処理
	VelocityUpdate();

	//---角度の設定---//
	// ハンドル操作の更新（旋回の適応など）
	handling_->PostUpdate(velocity_, status_);

	// タイヤの回転・向き決定
	std::vector<Car::IParts*> tires = {};
	tires = owner_->GetConstructionSystem()->FindPartsByCategory(1);
	for (std::vector<Car::IParts*>::iterator it = tires.begin(); it != tires.end(); ++it) {
		static_cast<TireParts*>((*it))->SetSpinRate(velocity_.z);
		static_cast<TireParts*>((*it))->SetSteerDirection(handling_->GetTireDirection());
		static_cast<TireParts*>((*it))->SetPreSteerDirection(handling_->GetPreTireDirection());
	}

	// 移動
	if (velocity_ != Vector3(0.0f, 0.0f, 0.0f))
	{
		// 向き
		Vector3 newDirect = Matrix4x4::TransformNormal(velocity_, Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), owner_->GetWorldTransformAdress()->direction_));
		// 座標計算
		owner_->GetWorldTransformAdress()->transform_.translate += newDirect * GameTimeSystem::GetInstance()->GetDeltaTime();
	}

	owner_->GetWorldTransformAdress()->transform_.translate = knockbackReactionController_->Execute();
	//// ノックバック
	//if (knockBack_ != Vector3(0.0f, 0.0f, 0.0f)) {
	//	float knockBackDecayFactor = 0.15f;
	//	knockBack_ = Ease::Easing(Ease::EaseName::Lerp, knockBack_, Vector3(0, 0, 0), knockBackDecayFactor);
	//	// 向き
	//	//Vector3 newDirect = Matrix4x4::TransformNormal(knockBack_, Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), owner_->GetWorldTransformAdress()->direction_));
	//	// 座標計算
	//	owner_->GetWorldTransformAdress()->transform_.translate += knockBack_ * GameTimeSystem::GetInstance()->GetDeltaTime();
	//}
}

void DriveSystem::PreUpdate()
{

}

void DriveSystem::InputAccept(GameKeyconfig* keyConfig)
{
	// エンジンの受付
	driveEngine_->EngineAccept(keyConfig);
	// ハンドルの入力受付
	handling_->HandleInput(keyConfig->GetLeftStick()->x);
}

void DriveSystem::ImGuiDraw()
{
	// ハンドル
	handling_->ImGuiDraw();
	// エンジン
	driveEngine_->ImGuiDraw();

	// 速度
	ImGui::DragFloat3("Velocity", &velocity_.x);
	ImGui::InputInt("PushCount", &pushCount_);

}

void DriveSystem::VelocityUpdate()
{
	//---速度の設定---//
	// 速度の計算
	// 速度レートが0の場合加算しない
	if (driveEngine_->GetAccumulatedAccel_() != 0) {
		Vector3 acceleration = Vector3(0.0f,0.0f,1.0f) * driveEngine_->GetAccumulatedAccel_();
		velocity_ += acceleration * GameTimeSystem::GetInstance()->GetDeltaTime();
	}

	//const float velocityDecrement = 0.7f;	// 減速値
	float friction = 1.0f;
	GlobalVariables* global = GlobalVariables::GetInstance();
	const float kDirtFriction = global->GetFloatValue("VehicleEngine", "DirtFriction");
	const float kRoadFriction = global->GetFloatValue("VehicleEngine", "RoadFriction");
	if (owner_->drivingLocation_ == CoursePolygonType::kCoursePolygonTypeDirt) {
		// ダートの方が大きい
		if (owner_->dirtCount_ > owner_->roadCount_) {
			// ダートのみ
			if (owner_->roadCount_ == 0) {
				friction = kDirtFriction;
			}
			else {
				friction = Ease::Easing(Ease::EaseName::Lerp, kRoadFriction, kDirtFriction, 0.75f);
			}
		}
		// 同じ
		else if (owner_->dirtCount_ == owner_->roadCount_) {
			friction = Ease::Easing(Ease::EaseName::Lerp, kRoadFriction, kDirtFriction, 0.5f);
		}
		// 道の方が大きい
		else {
			friction = Ease::Easing(Ease::EaseName::Lerp, kRoadFriction, kDirtFriction, 0.25f);
		}
	}
	else {
		friction = kRoadFriction;
	}
	// 摩擦減速
	velocity_ = velocity_ * (friction);


	// 0スナップ
	const float kEpsilon = 0.001f;	// 切り捨て値
	VehicleCaluclator calc;
	velocity_ = calc.SnapToZero(velocity_, kEpsilon);

}

void DriveSystem::PushPower(const Vector3& direction)
{
	// 押し出し入力
	knockbackReactionController_->OnEngineBroken(direction);
	//float powerFactor = 150.0f;
	//Vector3 power = direction;
	//power *= powerFactor;
	//// パワー向き
	//Vector3 powerDirection = Vector3(direction.x, 0, direction.z);
	//// 車体向き
	//Vector3 vehicleDirection = Vector3(owner_->GetWorldTransformAdress()->direction_.x, 0, owner_->GetWorldTransformAdress()->direction_.z);
	//
	//// 内積によって与える力を決めます
	//float directDot = Vector3::Dot(powerDirection, vehicleDirection);
	//// 後寄り
	//Vector3 addPower = power;
	//if (directDot <= -0.5f) {
	//	addPower *= (1.0f / 16.0f);
	//}
	//else if (directDot <= -0.25f) {
	//	addPower *= (1.0f / 8.0f);
	//}
	//else if (directDot <= 0.35f) {
	//	addPower *= (1.0f / 4.0f);
	//}
	//else if (directDot <= 0.65f) {
	//	addPower *= (1.0f / 2.0f);
	//}
	//else {
	//	addPower *= (1.0f / 1.0f);
	//}
	//
	//totalDirection_ += addPower;

	//float length = Vector3::Length(power);
	//// 向きに合わせる
	////owner_->GetWorldTransformAdress()->direction_ = Vector3::Normalize(power);
	//owner_->GetWorldTransformAdress()->direction_.y = 0.0f;
	//// 速度生成
	////velocity_ += Vector3(0.0f, 0.0f, 1.0f) * length;
	//if (!isPush_) {
	//	pushCount_++;
	//}
	//pushPower_ += Vector3(0.0f, 0.0f, 1.0f) * length;
	//pushVector_.second += power;
}
