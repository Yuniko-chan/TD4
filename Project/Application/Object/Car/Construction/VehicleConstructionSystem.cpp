#include "VehicleConstructionSystem.h"
#include "../CarLists.h"
#include "../Parts/System/PartsOffsetCalculator.h"

void VehicleConstructionSystem::Initialize(VehicleCore* core)
{
	core_ = core;
}

void VehicleConstructionSystem::Update()
{
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin(); it != partsMapping_.end(); ++it) {
		if ((*it).second->GetIsDelete()) {
			(*it).second->ReleaseParent();
			it = partsMapping_.erase(it);
		}
	}
}

void VehicleConstructionSystem::Attach(Car::IParts* parts)
{

	Matrix4x4 yRotate = Matrix4x4::MakeRotateYMatrix(core_->GetWorldTransformAdress()->transform_.rotate.y);
	// ベクトル
	Vector3 left = Matrix4x4::TransformNormal(Vector3::Normalize(Vector3(-1.0f, 0.0f, 0.0f)), yRotate);
	Vector3 right = Matrix4x4::TransformNormal(Vector3::Normalize(Vector3(1.0f, 0.0f, 0.0f)), yRotate);
	Vector3 forward = Matrix4x4::TransformNormal(Vector3::Normalize(Vector3(0.0f, 0.0f, 1.0f)), yRotate);
	Vector3 backForward = Matrix4x4::TransformNormal(Vector3::Normalize(Vector3(0.0f, 0.0f, -1.0f)), yRotate);
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
	Vector2Int mappingKey = {};
	const int kMaxGrid = 10;
	Car::IParts* preParts = nullptr;
	PartsOffsetCalculator calculator;
	// 親の設定
	core_->AddChild(parts);
	parts->GetWorldTransformAdress()->SetParent(core_->GetWorldTransformAdress());

	switch (direct)
	{
	case VehicleConstructionSystem::kLeft:
		for (int i = 1; i <= kMaxGrid; ++i) {
			mappingKey = { -i,0 };
			// 無ければ
			if (partsMapping_.find(mappingKey) == partsMapping_.end()) {
				parts->GetWorldTransformAdress()->transform_.translate = calculator.GetOffset(direct, i);
				// 深度
				parts->GetConnector()->SetDepth(i);
				// キー
				parts->GetConnector()->SetKey(Vector2((float)mappingKey.x, (float)mappingKey.y));
				// 消すフラグを初期化
				parts->SetIsDelete(false);
				// マッピング
				partsMapping_.emplace(mappingKey, parts);
				// 仮の処理
				if (i > 1) {
					i--;
					mappingKey = { -i,0 };
					preParts = partsMapping_.find(mappingKey)->second;
					parts->GetConnector()->AddParents(preParts);
				}
				else {
					parts->GetConnector()->AddParents(core_);
				}
				break;
			}
		}
		break;
	case VehicleConstructionSystem::kRight:
		for (int i = 1; i <= kMaxGrid; ++i) {
			mappingKey = { i,0 };
			// 無ければ
			if (partsMapping_.find(mappingKey) == partsMapping_.end()) {
				parts->GetWorldTransformAdress()->transform_.translate = calculator.GetOffset(direct, i);
				// 深度
				parts->GetConnector()->SetDepth(i);
				// キー
				parts->GetConnector()->SetKey(Vector2((float)mappingKey.x, (float)mappingKey.y));
				// 消すフラグを初期化
				parts->SetIsDelete(false);
				// マッピング
				partsMapping_.emplace(mappingKey, parts);
				// 仮の処理
				if (i > 1) {
					i--;
					mappingKey = { -i,0 };
					preParts = partsMapping_.find(mappingKey)->second;
					parts->GetConnector()->AddParents(preParts);
				}
				else {
					parts->GetConnector()->AddParents(core_);
				}
				break;
			}
		}
		break;
	case VehicleConstructionSystem::kForward:
		for (int i = 1; i <= kMaxGrid; ++i) {
			mappingKey = Vector2Int(0, i);
			// 無ければ
			if (partsMapping_.find(mappingKey) == partsMapping_.end()) {
				parts->GetWorldTransformAdress()->transform_.translate = calculator.GetOffset(direct, i);
				// 深度
				parts->GetConnector()->SetDepth(i);
				// キー
				parts->GetConnector()->SetKey(Vector2((float)mappingKey.x, (float)mappingKey.y));
				// 消すフラグを初期化
				parts->SetIsDelete(false);
				// マッピング
				partsMapping_.emplace(mappingKey, parts);
				// 仮の処理
				if (i > 1) {
					i--;
					mappingKey = { 0,i };
					preParts = partsMapping_.find(mappingKey)->second;
					parts->GetConnector()->AddParents(preParts);
				}
				else {
					parts->GetConnector()->AddParents(core_);
				}
				break;
			}
		}
		break;
	case VehicleConstructionSystem::kBackForward:
		for (int i = 1; i <= kMaxGrid; ++i) {
			mappingKey = Vector2Int(0, -i);
			// 無ければ
			if (partsMapping_.find(mappingKey) == partsMapping_.end()) {
				parts->GetWorldTransformAdress()->transform_.translate = calculator.GetOffset(direct, i);
				// 深度
				parts->GetConnector()->SetDepth(i);
				// キー
				parts->GetConnector()->SetKey(Vector2((float)mappingKey.x, (float)mappingKey.y));
				// 消すフラグを初期化
				parts->SetIsDelete(false);
				// マッピング
				partsMapping_.emplace(mappingKey, parts);
				// 仮の処理
				if (i > 1) {
					i--;
					mappingKey = { 0,-i };
					//std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.find(mappingKey);
					preParts = partsMapping_.find(mappingKey)->second;
					parts->GetConnector()->AddParents(preParts);
				}
				else {
					parts->GetConnector()->AddParents(core_);
				}
				break;
			}
		}
		break;
	}

}

Car::IParts* VehicleConstructionSystem::FindPreNumber(std::list<std::pair<int, Car::IParts*>>* directLists, int32_t number)
{
	for (std::list<std::pair<int, Car::IParts*>>::iterator it = directLists->begin(); it != directLists->end();) {
		// 該当番号が見つかれば
		if ((*it).first == number) {
			return (*it).second;
		}
		++it;
	}

	return nullptr;
}
