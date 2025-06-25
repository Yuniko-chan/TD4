#include "VehicleConstructionSystem.h"
#include "../../CarLists.h"

void VehicleConstructionSystem::Initialize()
{
	partsMapping_.emplace(Vector2Int(0, 0), owner_);
}

void VehicleConstructionSystem::Update()
{
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin(); it != partsMapping_.end(); ++it) {
		// 見つかった場合抜ける
		if ((*it).second->GetIsDelete()) {
			// 外す処理
			Detach(it);
			break;
		}
	}

	// 初期化
	emptyDatas_.clear();
	// データ作成
	for (std::vector<Vector2Int>::iterator it = emptyMap_.begin(); it != emptyMap_.end(); ++it) {
		VehicleCaluclator calc;
		Vector3 point = calc.GetIDToWorldPosition((*it), owner_->GetWorldTransformAdress());
		MappingKey data = { (*it),point };
		emptyDatas_.push_back(data);
	}

	// マップから
	status_->StatusUpdate(&partsMapping_);
}

void VehicleConstructionSystem::ImGuiDraw()
{
	int size = (int)emptyMap_.size();
	ImGui::InputInt("EmptyMapsize", &size);
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin();
		it != partsMapping_.end(); ++it) {
		ImGui::SeparatorText((*it).second->GetName().c_str());
		std::string name = (*it).second->GetName() + "Data";
		if (ImGui::TreeNode(name.c_str())) {
			// 座標表示
			name = (*it).second->GetName() + "Translate";
			EulerTransform t = (*it).second->GetWorldTransformAdress()->transform_;
			ImGui::DragFloat3(name.c_str(), &t.translate.x, 0.01f);

			// HP
			name = (*it).second->GetName() + "HP";
			float hp = (*it).second->GetHPHandler()->GetHP();
			ImGui::DragFloat(name.c_str(), &hp);

			// 配列キー
			name = (*it).second->GetName() + "Key";
			Vector2Int key = (*it).first;
			ImGui::InputInt2(name.c_str(), &key.x);

			// 深度
			name = (*it).second->GetName() + "Depth";
			int depth = (*it).second->GetConnector()->GetDepth();
			ImGui::InputInt(name.c_str(), &depth);

			ImGui::TreePop();
		}

		// 解除ボタン
		name = (*it).second->GetName() + "Release";
		if (ImGui::Button(name.c_str())) {
			(*it).second->GetHPHandler()->SetHP(0);
		}
	}
}

bool VehicleConstructionSystem::Attach(Car::IParts* parts)
{
	// 距離
	float distance = TransformHelper::Vector3Distance(
		parts->GetWorldTransformAdress()->GetWorldPosition(),
		owner_->GetWorldTransformAdress()->GetWorldPosition());
	// 対象セット
	std::pair<Vector2Int, Car::IParts*> futureParts = { Vector2Int(0,0),owner_ };

	// 各パーツとの距離比較検索
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
	Vector2Int newKey = Vector2Int();
	// 左
	if ((leftDot >= rightDot) && (leftDot >= forwardDot) && (leftDot >= backForwardDot)) {
		newKey = Vector2Int(futureParts.first.x - 1, futureParts.first.y);
	}
	// 右
	else if ((rightDot >= leftDot) && (rightDot >= forwardDot) && (rightDot >= backForwardDot)) {
		newKey = Vector2Int(futureParts.first.x + 1, futureParts.first.y);
	}
	// 前
	else if ((forwardDot >= leftDot) && (forwardDot >= rightDot) && (forwardDot >= backForwardDot)) {
		newKey = Vector2Int(futureParts.first.x, futureParts.first.y + 1);
	}
	// 手前
	else if ((backForwardDot >= leftDot) && (backForwardDot >= rightDot) && (backForwardDot >= forwardDot)) {
		newKey = Vector2Int(futureParts.first.x, futureParts.first.y - 1);
	}

	// 一致するものがなければ＆＆0,0でなければ
	if (!partsMapping_.contains(newKey) && newKey != Vector2Int(0, 0)) {
		Attach(parts, Vector2Int(newKey));

		// 空いてる場所
		this->emptyMap_ = VehicleCaluclator::GetEmptyList(&partsMapping_);

		return true;
	}

	// 見つからなかった場合失敗
	return false;
}

bool VehicleConstructionSystem::IsAttach(Car::IParts* parts, Vector2Int key)
{
	if (!partsMapping_.contains(key) && key != Vector2Int(0, 0)) {
		// アタッチ処理
		Attach(parts, Vector2Int(key));
		// 親の設定
		parts->SetParent(owner_);
		// 空マップ更新
		emptyMap_ = VehicleCaluclator::GetEmptyList(&partsMapping_);
		return true;
	}

	return false;
}

void VehicleConstructionSystem::AnyDocking(Car::IParts* parts, const Vector2Int& key)
{
	// 既にあればスキップ
	if (partsMapping_.contains(key) || key == Vector2Int(0,0)) {
		return;
	}
	// 追加
	Attach(parts, key);
	// 空いてる場所更新
	emptyMap_ = VehicleCaluclator::GetEmptyList(&partsMapping_);
}

Car::IParts* VehicleConstructionSystem::FindNearPart(const Vector3& point)
{
	// 一番近いパーツを返す
	float nearDist = FLT_MAX;
	Car::IParts* result = nullptr;

	// 
	for (auto it = partsMapping_.begin(); it != partsMapping_.end(); ++it) {
		float distance = TransformHelper::Vector3Distance(point, (*it).second->GetWorldTransformAdress()->GetWorldPosition());
		// 距離が短いか
		if (nearDist >= distance && (*it).second->GetClassNameString() != "VehicleCore") {
			nearDist = distance;
			result = (*it).second;
		}
	}

	// 
	return result;
}

void VehicleConstructionSystem::Attach(Car::IParts* parts, const Vector2Int& key)
{
	// 計算器
	PartsOffsetCalculator calculator;

	// HPのリセット処理
	parts->GetHPHandler()->Initialize();
	// 親子関係
	parts->GetWorldTransformAdress()->SetParent(owner_->GetWorldTransformAdress());
	// オフセット
	parts->GetWorldTransformAdress()->transform_.translate = calculator.GetOffset(key);
	// キー
	parts->GetConnector()->SetKey(Vector2((float)key.x, (float)key.y));
	// マッピング
	RegistParts(key, parts);
	if (parts->GetConnector()->GetDepth() <= 1) {
		parts->GetConnector()->AddParents(owner_);
	}

}

void VehicleConstructionSystem::Detach(std::map<Vector2Int, Car::IParts*>::iterator it)
{
	// キーが0,0ならスキップ
	if ((*it).first == Vector2Int(0, 0)) {
		return;
	}
	// 解除処理
	UnRegistParts((*it).first, (*it).second);
	// ステータス側からも削除
	status_->ApplyPartRemove((*it).second->GetClassNameString(), (*it).first);
	// HPの初期化
	(*it).second->GetHPHandler()->Initialize();
	// リストから外す
	it = partsMapping_.erase(it);
	// 空いてる場所更新
	emptyMap_ = VehicleCaluclator::GetEmptyList(&partsMapping_);
}

void VehicleConstructionSystem::Detach(Car::IParts* parts)
{
	// 検索
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin();
		it != partsMapping_.end(); ++it) {
		// 一致パーツへの処理
		if ((*it).second == parts) {
			// 外すためのフラグ処理
			(*it).second->SetIsDelete(true);
			// 共通の外す処理
			Detach(it);
			return;
		}
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

Car::IParts* VehicleConstructionSystem::FindParts(Car::IParts* parts)
{
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin(); it != partsMapping_.end();) {
		// 該当番号が見つかればそれを返す
		if ((*it).second == parts) {
			return (*it).second;
		}
		++it;
	}

	return nullptr;
}

std::vector<Car::IParts*> VehicleConstructionSystem::FindPartsByCategory(int typeID)
{
	std::vector<Car::IParts*> result = {};
	std::string className;

	// IDからクラス決定
	switch (typeID)
	{
	case 0:
		className = "EngineParts";
		break;
	case 1:
		className = "TireParts";
		break;
	case 2:
		className = "ArmorFrameParts";
		break;
	default:
		break;
	}

	// 名前から検索、追加
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin();
		it != partsMapping_.end(); ++it) {
		if ((*it).second->GetClassNameString() == className) {
			result.push_back((*it).second);
		}
	}

	return std::vector<Car::IParts*>(result);
}

void VehicleConstructionSystem::RegistParts(const Vector2Int& id, Car::IParts* parts)
{
	// カウント追加
	status_->ApplyPartAdd(parts->GetClassNameString(), id);

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
	// 最小深度値
	int minDepth = 100;
	// 一個しかなければ
	bool isOne = adjoinParts.size();
	// 子・親の登録
	for (std::list<Car::IParts*>::iterator it = adjoinParts.begin(); it != adjoinParts.end(); ++it) {
		// 数による処理別
		if (isOne) {
			// 数が一つかつそれがコアである場合
			if ((*it)->GetClassNameString() == "VehicleCore") {
				parts->GetConnector()->AddParents(*it);
				parts->GetConnector()->SetDepth(1);
				break;
			}

			// コアでない場合（親登録＋深度値を親＋１で決定）
			parts->GetConnector()->AddParents(*it);
			parts->GetConnector()->SetDepth((*it)->GetConnector()->GetDepth() + 1);
			break;
		}

		// コアなら親として登録のみ行う
		if ((*it)->GetClassNameString() == "VehicleCore") {
			parts->GetConnector()->AddParents(*it);
			continue;
		}
		// 最小の設定
		if (minDepth > (*it)->GetConnector()->GetDepth()) {
			minDepth = (*it)->GetConnector()->GetDepth();
		}

		//// 対象の深度値
		//int32_t targetDepth = (*it)->GetConnector()->GetDepth();
		//// 子に追加
		//if (parts->GetConnector()->GetDepth() < targetDepth) {
		//	parts->GetConnector()->AddChildren(*it);
		//	(*it)->GetConnector()->AddParents(parts);
		//}
		//// 親に追加
		//else if (parts->GetConnector()->GetDepth() > targetDepth) {
		//	parts->GetConnector()->AddParents(*it);
		//	// 子に設定
		//	(*it)->GetConnector()->AddChildren(parts);
		//}
	}
	// 一個しかなければ早期
	if (isOne) {
		return;
	}

	// 深度設定
	parts->GetConnector()->SetDepth(minDepth);

	for (std::list<Car::IParts*>::iterator it = adjoinParts.begin(); it != adjoinParts.end(); ++it) {
		// コアなら親として登録のみ行う
		if ((*it)->GetClassNameString() == "VehicleCore") {
			parts->GetConnector()->AddParents(*it);
			continue;
		}
		// 対象の深度値
		int32_t targetDepth = parts->GetConnector()->GetDepth();
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
		// コアならスキップ
		if ((*it)->GetClassNameString() == "VehicleCore") {
			continue;
		}
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
