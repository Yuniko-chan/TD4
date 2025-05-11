#include "VehicleConstructionSystem.h"
#include "../CarLists.h"
#include "../Parts/System/PartsOffsetCalculator.h"

void VehicleConstructionSystem::Initialize(VehicleCore* core)
{
	core_ = core;
}

void VehicleConstructionSystem::Update()
{

	for (std::list<std::pair<int, Car::IParts*>>::iterator it = leftD_.begin(); it != leftD_.end(); ++it) {
		if ((*it).second->GetIsDelete()) {

		}
	}
	for (std::list<std::pair<int, Car::IParts*>>::iterator it = rightD_.begin(); it != rightD_.end(); ++it) {
		if ((*it).second->GetIsDelete()) {

		}
	}
	for (std::list<std::pair<int, Car::IParts*>>::iterator it = forwardD_.begin(); it != forwardD_.end(); ++it) {
		if ((*it).second->GetIsDelete()) {

		}
	}
	for (std::list<std::pair<int, Car::IParts*>>::iterator it = backForwardD_.begin(); it != backForwardD_.end(); ++it) {
		if ((*it).second->GetIsDelete()) {

		}
	}

}

void VehicleConstructionSystem::Attach(Car::IParts* parts)
{
	// ベクトル
	Vector3 left = Vector3::Normalize(Vector3(-1.0f, 0.0f, 0.0f));
	Vector3 right = Vector3::Normalize(Vector3(1.0f, 0.0f, 0.0f));
	Vector3 forward = Vector3::Normalize(Vector3(1.0f, 0.0f, 0.0f));
	Vector3 backForward = Vector3::Normalize(Vector3(-1.0f, 0.0f, 0.0f));
	Vector3 toDirect = parts->GetWorldTransformAdress()->GetWorldPosition() - core_->GetWorldTransformAdress()->GetWorldPosition();
	toDirect = Vector3::Normalize({ toDirect.x,0.0f,toDirect.z });
	// 方向の内積
	float leftDot = Vector3::Dot(left, toDirect);
	float rightDot = Vector3::Dot(right, toDirect);
	float forwardDot = Vector3::Dot(forward, toDirect);
	float backForwardDot = Vector3::Dot(backForward, toDirect);
	// 左
	if ((leftDot >= rightDot) && (leftDot >= forwardDot) && (leftDot >= backForwardDot)) {
		Attach(parts, Direction::kLeft);
	}
	// 右
	else if ((rightDot >= leftDot) && (rightDot >= forwardDot) && (rightDot >= backForwardDot)) {
		Attach(parts, Direction::kRight);
	}
	// 前
	else if ((forwardDot >= leftDot) && (forwardDot >= rightDot) && (forwardDot >= backForwardDot)) {
		Attach(parts, Direction::kForward);
	}
	// 手前
	else if ((backForwardDot >= leftDot) && (backForwardDot >= rightDot) && (backForwardDot >= forwardDot)) {
		Attach(parts, Direction::kBackForward);
	}
}

void VehicleConstructionSystem::Attach(Car::IParts* parts, Direction direct)
{
	int number = 0;
	PartsOffsetCalculator calculator;

	switch (direct)
	{
	case VehicleConstructionSystem::kLeft:
		number = (int)leftD_.size();
		number++;
		leftD_.push_back({ number ,parts });

		

		break;
	case VehicleConstructionSystem::kRight:
		number = (int)rightD_.size();
		number++;
		rightD_.push_back({ number ,parts });
		break;
	case VehicleConstructionSystem::kForward:
		number = (int)forwardD_.size();
		number++;
		forwardD_.push_back({ number ,parts });
		break;
	case VehicleConstructionSystem::kBackForward:
		number = (int)backForwardD_.size();
		number++;
		backForwardD_.push_back({ number ,parts });
		break;
	default:
		break;
	}
	// 親の設定
	core_->AddChild(parts);
	//parts->SetParent(core_);
	parts->GetWorldTransformAdress()->SetParent(core_->GetWorldTransformAdress());
	parts->GetWorldTransformAdress()->transform_.translate = calculator.GetOffset(direct, number);

}
