#include "DriveSystem.h"
#include "../../VehicleCore.h"
#include "../../CarLists.h"
#include "../../../GameTimer/GameTimeSystem.h"

DriveSystem::DriveSystem()
{
	// エンジン
	driveEngine_ = std::make_unique<DriveEngine>();
	// ハンドル
	handling_ = std::make_unique<DriveHandling>();
}

void DriveSystem::Initialize()
{
	// ハンドルクラス
	handling_->SetOwner(owner_);

	// エンジングラス
	driveEngine_->SetOwner(owner_);

	slowTimer_.End();
}

void DriveSystem::Update()
{
	slowTimer_.Update(1.0f / GameTimeSystem::GetInstance()->GetTimeScale());
	if (pushCount_ != 0) {
		isPush_ = true;
	}
	if (isPush_) {
		pushVector_.second = Vector3::Normalize(pushVector_.second);
		pushVector_.second.y = 0.0f;
		pushVector_.first = owner_->GetWorldTransformAdress()->direction_;
		pushVector_.first.y = 0.0f;
		//Vector3 newPush = Matrix4x4::TransformNormal(pushPower_, Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), pushVector_.second));
		//owner_->GetWorldTransformAdress()->transform_.translate += newPush * GameTimeSystem::GetInstance()->GetDeltaTime();
		//pushPower_ = {};
		isPush_ = false;
		pushCount_ = 0;
		// 
		if (!slowTimer_.IsActive()) {
			slowTimer_.Start(1.0f);
			GameTimeSystem::GetInstance()->SetTimeScale(0.1f);
		}
	}

	if (slowTimer_.IsActive()) {
		//Quaternion from = Quaternion::DirectionToDirection(Vector3(0, 0, 1), GetPushDirection().first);
		//Quaternion to = Quaternion::DirectionToDirection(Vector3(0, 0, 1), GetPushDirection().second);
		//float t = slowTimer_.GetElapsedFrame();
		//Quaternion slerp = Quaternion::Slerp(from, to, t);
		//Vector3 newDirect = Quaternion::RotateVector(Vector3(0, 0, 1), slerp);
		//owner_->GetWorldTransformAdress()->direction_ = Vector3::Normalize(newDirect);
		owner_->GetWorldTransformAdress()->direction_ = pushVector_.second;
	}
	if (slowTimer_.IsEnd()) {
		GameTimeSystem::GetInstance()->SetTimeScale(1.0f);
		velocity_ += pushPower_;
		pushPower_ = {};
	}

	// オーバーヒートフラグ初期化
	status_->SetIsOverheat(false);

	//---それぞれのシステム処理---//
	// ハンドル処理
	handling_->SetVehicleDirection(owner_->GetWorldTransformAdress()->direction_);
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
		static_cast<TireParts*>((*it))->SetSteerDirection(handling_->GetSteerDirection());
		static_cast<TireParts*>((*it))->SetPreSteerDirection(handling_->GetPreSteerDirection());
	}

	// 速度が無ければ早期
	if (velocity_ == Vector3(0.0f, 0.0f, 0.0f))
	{
		return;
	}

	// 角度
	//float eulerY = TransformHelper::CalculateXZVectorToRotateRadian(owner_->GetWorldTransformAdress()->direction_, Vector3::FrontVector());
	Vector3 newDirect = Matrix4x4::TransformNormal(velocity_, Matrix4x4::DirectionToDirection(Vector3(0.0f, 0.0f, 1.0f), owner_->GetWorldTransformAdress()->direction_));
	
	// 座標計算
	//VehicleCaluclator calc;
	owner_->GetWorldTransformAdress()->transform_.translate += newDirect * GameTimeSystem::GetInstance()->GetDeltaTime();

	// 
	if (!owner_->IsPlayer()) {
		HandleNoParent();
	}

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
	// 速度
	ImGui::DragFloat3("Velocity", &velocity_.x);
	ImGui::InputInt("PushCount", &pushCount_);
	// ハンドル
	handling_->ImGuiDraw();
	// エンジン
	driveEngine_->ImGuiDraw();
}

void DriveSystem::VelocityUpdate()
{
	//---速度の設定---//
	// 速度の計算
	// 速度レートが0の場合加算しない
	if (driveEngine_->GetCurrentSpeed() != 0) {
		Vector3 acceleration = Vector3::FrontVector() * driveEngine_->GetCurrentSpeed();
		velocity_ += acceleration * GameTimeSystem::GetInstance()->GetDeltaTime();
	}

	const float velocityDecrement = 0.75f;	// 減速値
	const float kEpsilon = 0.001f;	// 切り捨て値
	// 減速
	velocity_ = velocity_ * velocityDecrement;
	// 0に調節
	VehicleCaluclator calc;
	velocity_ = calc.SnapToZero(velocity_, kEpsilon);

}

void DriveSystem::HandleNoParent()
{
	if (velocity_.z == 0.0f) {
		handling_->SetVehicleDirection(Vector3(0.0f, 0.0f, 1.0f));
	}
}

void DriveSystem::PushPower(const Vector3& power)
{
	float length = Vector3::Length(power);
	// 向きに合わせる
	//owner_->GetWorldTransformAdress()->direction_ = Vector3::Normalize(power);
	owner_->GetWorldTransformAdress()->direction_.y = 0.0f;
	// 速度生成
	//velocity_ += Vector3(0.0f, 0.0f, 1.0f) * length;
	if (!isPush_) {
		pushCount_++;
	}
	pushPower_ += Vector3(0.0f, 0.0f, 1.0f) * length;
	pushVector_.second += power;
}
