#include "VehicleConstructionSystem.h"
#include "../CarLists.h"
#include "../Parts/System/PartsOffsetCalculator.h"
#include "../../Utility/Calc/TransformHelper.h"

#include "../../../Engine/2D/ImguiManager.h"

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
			DeleteCount((*it).second->GetClassNameString());
			it = partsMapping_.erase(it);
			break;
		}
	}
	// 各方向の数取得
	directions_ = {};
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin(); it != partsMapping_.end(); ++it) {
		if ((*it).first.x > 0) {
			directions_.right++;
		}
		else if ((*it).first.x < 0) {
			directions_.left++;
		}

		if ((*it).first.y > 0)
		{
			directions_.forward++;
		}
		else if ((*it).first.y < 0) {
			directions_.backForward++;
		}
	}

}

void VehicleConstructionSystem::ImGuiDraw()
{
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin();
		it != partsMapping_.end(); ++it) {
		ImGui::SeparatorText((*it).second->GetName().c_str());
		std::string name = (*it).second->GetName() + "Data";
		if (ImGui::TreeNode(name.c_str())) {
			// 座標表示
			name = (*it).second->GetName() + "Translate";
			EulerTransform t = (*it).second->GetWorldTransformAdress()->transform_;
			ImGui::DragFloat3(name.c_str(), &t.translate.x, 0.01f);
			// 配列キー
			name = (*it).second->GetName() + "Key";
			Vector2Int key = (*it).first;
			ImGui::InputInt2(name.c_str(), &key.x);

			ImGui::TreePop();
		}

		// 解除ボタン
		name = (*it).second->GetName() + "Release";
		if (ImGui::Button(name.c_str())) {
			(*it).second->SetHP(0);
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

void VehicleConstructionSystem::Attach(Car::IParts* parts, const Vector2Int& key)
{
	// 計算器
	PartsOffsetCalculator calculator;

	// HPのリセット処理
	parts->SetHP(1);
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
	// カウント追加
	this->AddCount(parts->GetClassNameString());
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
	//// カウントから削除
	//DeleteCount(parts->GetClassNameString());
}

void VehicleConstructionSystem::AddCount(std::string name)
{
	if (name == "TireParts") {
		counts_.tire++;
	}
	else if (name == "ArmorFrameParts") {
		counts_.armor++;
	}
	else if (name == "EngineParts") {
		counts_.engine++;
	}
}

void VehicleConstructionSystem::DeleteCount(std::string name)
{
	if (name == "TireParts") {
		counts_.tire--;
	}
	else if (name == "ArmorFrameParts") {
		counts_.armor--;
	}
	else if (name == "EngineParts") {
		counts_.engine--;
	}

	if (counts_.tire < 0) {
		counts_.tire = 0;
	}
	if (counts_.armor < 0) {
		counts_.armor = 0;
	}
	if (counts_.engine < 0) {
		counts_.engine = 0;
	}
}
