#include "VehicleConstructionSystem.h"
#include "../CarLists.h"
#include "../Parts/System/PartsOffsetCalculator.h"
#include "../../Utility/Calc/TransformHelper.h"

void VehicleConstructionSystem::Initialize(VehicleCore* core)
{
	core_ = core;
}

void VehicleConstructionSystem::Update()
{
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin(); it != partsMapping_.end(); ++it) {
		if ((*it).second->GetIsDelete()) {
			UnRegistParts((*it).first, (*it).second);
			//(*it).second->ReleaseParent();
			it = partsMapping_.erase(it);
			break;
		}
	}
}

void VehicleConstructionSystem::Attach(Car::IParts* parts)
{
	// 距離
	float distance = TransformHelper::Vector3Distance(
		parts->GetWorldTransformAdress()->GetWorldPosition(),
		core_->GetWorldTransformAdress()->GetWorldPosition());
	// 対象セット
	std::pair<Vector2Int, Car::IParts*> futureParts = { Vector2Int(0,0),core_ };

	// 既に付いているパーツの検索
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin();
		it != partsMapping_.end(); ++it) {
		float newDistace = TransformHelper::Vector3Distance(
			parts->GetWorldTransformAdress()->GetWorldPosition(),
			(*it).second->GetWorldTransformAdress()->GetWorldPosition());
		// 最小距離の計算
		if (newDistace <= distance) {
			distance = newDistace;
			// セットで保存できるように
			futureParts.first = Vector2Int((*it).first);
			futureParts.second = (*it).second;
		}
	}

	Matrix4x4 yRotate = Matrix4x4::MakeRotateYMatrix(futureParts.second->GetWorldTransformAdress()->transform_.rotate.y);
	// ベクトル
	Vector3 left = Matrix4x4::TransformNormal(Vector3::Normalize(Vector3(-1.0f, 0.0f, 0.0f)), yRotate);
	Vector3 right = Matrix4x4::TransformNormal(Vector3::Normalize(Vector3(1.0f, 0.0f, 0.0f)), yRotate);
	Vector3 forward = Matrix4x4::TransformNormal(Vector3::Normalize(Vector3(0.0f, 0.0f, 1.0f)), yRotate);
	Vector3 backForward = Matrix4x4::TransformNormal(Vector3::Normalize(Vector3(0.0f, 0.0f, -1.0f)), yRotate);
	Vector3 toDirect = parts->GetWorldTransformAdress()->GetWorldPosition() - futureParts.second->GetWorldTransformAdress()->GetWorldPosition();
	toDirect = Vector3::Normalize({ toDirect.x,0.0f,toDirect.z });
	// 方向の内積
	float leftDot = Vector3::Dot(left, toDirect);
	float rightDot = Vector3::Dot(right, toDirect);
	float forwardDot = Vector3::Dot(forward, toDirect);
	float backForwardDot = Vector3::Dot(backForward, toDirect);
	// 左
	if ((leftDot >= rightDot) && (leftDot >= forwardDot) && (leftDot >= backForwardDot)) {
		if (!partsMapping_.contains(Vector2Int(futureParts.first.x - 1, futureParts.first.y))) {
			Attach(parts, Vector2Int(futureParts.first.x - 1, futureParts.first.y));
		}
	}
	// 右
	else if ((rightDot >= leftDot) && (rightDot >= forwardDot) && (rightDot >= backForwardDot)) {
		if (!partsMapping_.contains(Vector2Int(futureParts.first.x + 1, futureParts.first.y))) {
			Attach(parts, Vector2Int(futureParts.first.x + 1, futureParts.first.y));
		}
	}
	// 前
	else if ((forwardDot >= leftDot) && (forwardDot >= rightDot) && (forwardDot >= backForwardDot)) {
		if (!partsMapping_.contains(Vector2Int(futureParts.first.x, futureParts.first.y + 1))) {
			Attach(parts, Vector2Int(futureParts.first.x, futureParts.first.y + 1));
		}
	}
	// 手前
	else if ((backForwardDot >= leftDot) && (backForwardDot >= rightDot) && (backForwardDot >= forwardDot)) {
		if (!partsMapping_.contains(Vector2Int(futureParts.first.x, futureParts.first.y - 1))) {
			Attach(parts, Vector2Int(futureParts.first.x, futureParts.first.y - 1));
		}
	}
}

void VehicleConstructionSystem::Attach(Car::IParts* parts, Direction direct)
{
	Vector2Int mappingKey = {};
	const int kMaxGrid = 10;
	//Car::IParts* preParts = nullptr;
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
				RegistParts(mappingKey, parts);
				if (mappingKey.GetLength() <= 1) {
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
				RegistParts(mappingKey, parts);
				if (mappingKey.GetLength() <= 1) {
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
				RegistParts(mappingKey, parts);
				if (mappingKey.GetLength() <= 1) {
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
				RegistParts(mappingKey, parts);
				if (mappingKey.GetLength() <= 1) {
					parts->GetConnector()->AddParents(core_);
				}
				break;
			}
		}
		break;
	}

}

void VehicleConstructionSystem::Attach(Car::IParts* parts, const Vector2Int& key)
{
	// 計算器
	PartsOffsetCalculator calculator;

	// 親の設定
	core_->AddChild(parts);
	// 親子関係
	parts->GetWorldTransformAdress()->SetParent(core_->GetWorldTransformAdress());
	// オフセット
	parts->GetWorldTransformAdress()->transform_.translate = calculator.GetOffset(key);
	// 深度
	parts->GetConnector()->SetDepth(key.GetLength());
	// キー
	parts->GetConnector()->SetKey(Vector2((float)key.x, (float)key.y));
	// 消すフラグを初期化
	parts->SetIsDelete(false);
	// マッピング
	RegistParts(key, parts);
	if (parts->GetConnector()->GetDepth() <= 1) {
		parts->GetConnector()->AddParents(core_);
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

void VehicleConstructionSystem::RegistParts(const Vector2Int& id, Car::IParts* parts)
{
	// リストに登録
	partsMapping_.emplace(id, parts);
	// 隣接検索
	std::list<Car::IParts*> adjoinParts;
	Vector2Int findID = {};
	findID = Vector2Int(id.x + 1, id.y);
	if (partsMapping_.contains(findID)) {
		adjoinParts.push_back(partsMapping_.find(findID)->second);
	}
	findID = Vector2Int(id.x - 1, id.y);
	if (partsMapping_.contains(findID)) {
		adjoinParts.push_back(partsMapping_.find(findID)->second);
	}
	findID = Vector2Int(id.x, id.y + 1);
	if (partsMapping_.contains(findID)) {
		adjoinParts.push_back(partsMapping_.find(findID)->second);
	}
	findID = Vector2Int(id.x, id.y - 1);
	if (partsMapping_.contains(findID)) {
		adjoinParts.push_back(partsMapping_.find(findID)->second);
	}
	// 子・親の登録
	for (std::list<Car::IParts*>::iterator it = adjoinParts.begin(); it != adjoinParts.end(); ++it) {
		// 対象の深度値
		int32_t targetDepth = (*it)->GetConnector()->GetDepth();
		// 子に追加
		if (parts->GetConnector()->GetDepth() < targetDepth) {
			parts->GetConnector()->AddChildren(*it);
			(*it)->GetConnector()->AddParents(parts);
		}
		// 親に追加
		else if (parts->GetConnector()->GetDepth() > targetDepth) {
			parts->GetConnector()->AddParents(*it);
			// 子に設定
			(*it)->GetConnector()->AddChildren(parts);
		}
	}

}

void VehicleConstructionSystem::UnRegistParts(const Vector2Int& id, Car::IParts* parts)
{
	// 隣接を検索
	std::list<Car::IParts*> adjoinParts;
	// IDから探す
	Vector2Int findID = {};
	findID = Vector2Int(id.x + 1, id.y);
	if (partsMapping_.contains(findID)) {
		adjoinParts.push_back(partsMapping_.find(findID)->second);
	}
	findID = Vector2Int(id.x - 1, id.y);
	if (partsMapping_.contains(findID)) {
		adjoinParts.push_back(partsMapping_.find(findID)->second);
	}
	findID = Vector2Int(id.x, id.y + 1);
	if (partsMapping_.contains(findID)) {
		adjoinParts.push_back(partsMapping_.find(findID)->second);
	}
	findID = Vector2Int(id.x, id.y - 1);
	if (partsMapping_.contains(findID)) {
		adjoinParts.push_back(partsMapping_.find(findID)->second);
	}

	// 隣接パーツ（子・親）の解除処理
	for (std::list<Car::IParts*>::iterator it = adjoinParts.begin(); it != adjoinParts.end(); ++it) {
		// 対象の深度値
		int32_t targetDepth = (*it)->GetConnector()->GetDepth();
		// 自分が子である場合
		if (parts->GetConnector()->GetDepth() > targetDepth) {
			(*it)->GetConnector()->DeleteChildren(parts);
		}
		// 自分が親である場合
		else if (parts->GetConnector()->GetDepth() < targetDepth) {
			// 
			(*it)->GetConnector()->ReleaseParent(parts);
		}
	}

	// パーツ解除処理
	parts->ReleaseParent();

}
