#include "VehicleConstructionSystem.h"
#include "../../CarLists.h"

#include <queue>

void VehicleConstructionSystem::Initialize()
{
	partsMapping_.emplace(Vector2Int(0, 0), owner_);
}

void VehicleConstructionSystem::Update()
{
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin(); it != partsMapping_.end();) {
		// 見つかった場合抜ける
		if ((*it).second->GetIsDelete()) {
			// 外す処理
			Detach(it);
			it = partsMapping_.begin();
		}
		else {
			it++;
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
	ImGui::InputInt2("GridSize", &maxGridSize_.first);

	if (ImGui::Button("RefreshDepth")) {
		RefrashDepthsFromCore();
		RefrashPartsConnector();
	}

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
	// トランスフォーム
	// 親子関係
	parts->GetWorldTransformAdress()->SetParent(owner_->GetWorldTransformAdress());
	parts->GetWorldTransformAdress()->transform_.translate = calculator.GetOffset(key);
	parts->GetWorldTransformAdress()->transform_.scale = Vector3(1.0f, 1.0f, 1.0f);
	// キー
	parts->GetConnector()->SetKey(Vector2((float)key.x, (float)key.y));
	// マッピング
	RegistParts(key, parts);
	if (parts->GetConnector()->GetDepth() <= 1) {
		parts->GetConnector()->AddParents(owner_);
	}

	// グリッドのリフレッシュ
	RefrashGridSize();
}

void VehicleConstructionSystem::Detach(std::map<Vector2Int, Car::IParts*>::iterator it)
{
	// キーが0,0ならスキップ
	if ((*it).first == Vector2Int(0, 0)) {
		return;
	}
	if ((*it).second->GetClassNameString() == "EngineParts") {
		if ((*it).second->GetHPHandler()->IsDead()) {
			// 爆破解除
			BombUnRegistParts((*it).first, (*it).second);
		}
		else {
			// 解除処理
			UnRegistParts((*it).first, (*it).second);
		}
	}
	else {
		// 解除処理
		UnRegistParts((*it).first, (*it).second);
	}
	// 共通
	DetachCommon(it);
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

void VehicleConstructionSystem::RefrashDepthsFromCore()
{
	for (auto it = partsMapping_.begin(); it != partsMapping_.end(); ++it) {
		if ((*it).second->GetClassNameString() == "VehicleCore") continue;
		(*it).second->GetConnector()->SetDepth(-1);
	}

	// キュー
	std::queue<std::pair<Vector2Int, int>> queue;
	// 四方向
	const Vector2Int kDirections[4] = { {1,0},{-1,0},{0,1},{0,-1} };
	// 深度が1のみ例外
	for (const Vector2Int& direct : kDirections) {
		std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.find(direct);
		// 最後なら
		if (it == partsMapping_.end()) continue;
		// 深度設定
		(*it).second->GetConnector()->SetDepth(1);
		// 幅探索用のキューに
		queue.push({ direct, 1 });
	}

	while (!queue.empty()) {
		std::pair<Vector2Int, int> nowKey = queue.front();
		queue.pop();
		// 現在値
		//Car::IParts* currentPart = partsMapping_[nowKey.first];
		for (const Vector2Int& direct : kDirections) {
			// 検索中のキー
			Vector2Int neighborPoint = nowKey.first + Vector2Int(direct);
			// マップにあるか
			std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.find(neighborPoint);
			// 無かったら次
			if (it == partsMapping_.end()) continue;
			if (neighborPoint == Vector2Int(0, 0)) continue;

			// 見つかったパーツの深度
			int neighborDepth = (*it).second->GetConnector()->GetDepth();
			// 新しい深度
			int expectedDepth = nowKey.second + 1;

			// 隣接深度が今より高ければ｜｜-1（初期値）だった場合
			if (neighborDepth >= expectedDepth || neighborDepth == -1) {
				// 深度設定
				(*it).second->GetConnector()->SetDepth(expectedDepth);
				// 幅探索用のキューに
				queue.push({ neighborPoint, expectedDepth });
			}
		}
	}
}

void VehicleConstructionSystem::RefrashPartsConnector()
{
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin();
		it != partsMapping_.end(); ++it) {
		if ((*it).second->GetClassNameString() != "VehicleCore") {
			(*it).second->GetConnector()->Reset();
		}
	}
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin();
		it != partsMapping_.end(); ++it) {
		if ((*it).second->GetClassNameString() == "VehicleCore") {
			continue;
		}
		// 隣接検索
		std::list<Car::IParts*> adjoinParts;
		Vector2Int findID = {};
		Vector2Int nowKey = (*it).first;
		findID = Vector2Int(nowKey.x + 1, nowKey.y);
		if (partsMapping_.contains(findID)) {
			adjoinParts.push_back(partsMapping_.find(findID)->second);
		}
		findID = Vector2Int(nowKey.x - 1, nowKey.y);
		if (partsMapping_.contains(findID)) {
			adjoinParts.push_back(partsMapping_.find(findID)->second);
		}
		findID = Vector2Int(nowKey.x, nowKey.y + 1);
		if (partsMapping_.contains(findID)) {
			adjoinParts.push_back(partsMapping_.find(findID)->second);
		}
		findID = Vector2Int(nowKey.x, nowKey.y - 1);
		if (partsMapping_.contains(findID)) {
			adjoinParts.push_back(partsMapping_.find(findID)->second);
		}
		
		// 子・親の登録
		for (std::list<Car::IParts*>::iterator adjoinIt = adjoinParts.begin(); adjoinIt != adjoinParts.end(); ++adjoinIt) {
			// コアだったら
			if ((*adjoinIt)->GetClassNameString() == "VehicleCore") {
				(*it).second->GetConnector()->AddParents(*adjoinIt);
				continue;
			}

			int currentDepth = (*it).second->GetConnector()->GetDepth();
			int targetDepth = (*adjoinIt)->GetConnector()->GetDepth();
			// （自）大きければ：親
			if (currentDepth > targetDepth) {
				(*it).second->GetConnector()->AddParents((*adjoinIt));
				(*adjoinIt)->GetConnector()->AddChildren((*it).second);
			}
			// （自）小さければ：子
			else if (currentDepth < targetDepth) {
				(*it).second->GetConnector()->AddChildren((*adjoinIt));
				(*adjoinIt)->GetConnector()->AddParents((*it).second);
			}
		}
	}
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

	// マッピング関係のリフレッシュ
	RefrashGridSize();
	RefrashDepthsFromCore();
	RefrashPartsConnector();

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

	//// カウントから削除
	//DeleteCount(parts->GetClassNameString());
}

void VehicleConstructionSystem::BombUnRegistParts(const Vector2Int& id, Car::IParts* parts)
{
	Vector3 direct = owner_->GetWorldTransformAdress()->GetWorldPosition() - parts->GetWorldTransformAdress()->GetWorldPosition();
	const float kPusher = 150.0f;
	owner_->GetDriveSystem()->PushPower(Vector3::Normalize(direct) * kPusher);

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

	for (std::list<Car::IParts*>::iterator it = adjoinParts.begin(); it != adjoinParts.end(); it++) {
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
		// 隣接を解除
		(*it)->SetIsDelete(true);
		// エンジンなら連鎖
		if ((*it)->GetClassNameString() == "EngineParts") {
			(*it)->GetHPHandler()->SetHP(0);
			(*it)->GetHPHandler()->Update();
		}

	}

}

void VehicleConstructionSystem::DetachCommon(std::map<Vector2Int, Car::IParts*>::iterator it)
{
	// 親子解除
	(*it).second->ReleaseParent();
	// ステータス側からも削除
	status_->ApplyPartRemove((*it).second->GetClassNameString(), (*it).first);
	// 解除時のアクション処理
	(*it).second->OnDetach();
	// HPの初期化
	(*it).second->GetHPHandler()->Initialize();
	// リストから外す
	it = partsMapping_.erase(it);
	// マッピング関係のリフレッシュ
	RefrashGridSize();
	RefrashDepthsFromCore();
	RefrashPartsConnector();

	// 空いてる場所更新
	emptyMap_ = VehicleCaluclator::GetEmptyList(&partsMapping_);

}

void VehicleConstructionSystem::RefrashGridSize()
{
	GridSize maxSize = {};
	for (std::map<Vector2Int, Car::IParts*>::iterator it = partsMapping_.begin();
		it != partsMapping_.end(); ++it) {
		if (std::abs((*it).first.x) > maxSize.first) {
			maxSize.first = std::abs((*it).first.x);
		}
		if (std::abs((*it).first.y) > maxSize.second) {
			maxSize.second = std::abs((*it).first.y);
		}
	}

	this->maxGridSize_ = maxSize;
}
