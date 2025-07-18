#include "PlayerPickupManager.h"

#include "../Player.h"
#include "../State/PlayerStatesList.h"

#include "../../Car/CarLists.h"
#include "../../Car/Manager/VehiclePartsManager.h"
#include "../../Car/Manager/PickupPointManager.h"
#include "../../Car/PickupPoint/InterfacePickupPoint.h"
#include "../../Utility/Calc/TransformHelper.h"
#include "../../Interact/InteractionSpot.h"

#include "../../../Engine/2D/ImguiManager.h"

#include <typeinfo>

PlayerPickupManager::PlayerPickupManager()
{
}

void PlayerPickupManager::Initialize()
{
	judgeSystem_ = std::make_unique<PartJudgeSystem>();
	judgeSystem_->SetOwner(owner_);
	// 置ける
	attachInteract_ = std::make_unique<AttachVisualizer>();
	attachInteract_->Initialize(owner_);
	// 拾える
	pickupInteract_ = std::make_unique<PickupVisualizer>();
	pickupInteract_->Initialize(owner_);
}

void PlayerPickupManager::Update()
{
	// タイマー
	if (interactDuration_.has_value()) {
		// 更新
		interactDuration_->Update();
		// 終了タイミング
		if (interactDuration_->IsEnd()) {
			interactDuration_ = std::nullopt;
		}
	}
	// インタラクト更新
	pickupInteract_->Update();

	// 車体に乗っている時は処理をスキップ
	if (owner_->GetStateMachine()->GetCurrentState()) {
		const std::type_info& id = typeid(*owner_->GetStateMachine()->GetCurrentState());
		std::string stateName = id.name();
		if (stateName == "class PlayerInVehicleState") {
			return;
		}
	}

	if (holdParts_) {
		// ピックアップ場所表示
		PickupVisualizer* pickUp = static_cast<PickupVisualizer*>(pickupInteract_.get());
		if (pickUp->IsParent()) {
			pickUp->SetTransform(nullptr);
			pickUp->Refresh();
		}

		// コアがなければスキップ
		if (!owner_->GetCore()) {
			return;
		}
		// コアなら処理を行わない
		if (holdParts_->GetClassNameString() == "VehicleCore") {
			return;
		}

		// 設置場所表示
		VehicleCaluclator calc;
		std::pair<Vector2Int, Vector3> nearPoint = calc.GetEmptyToNearPoint(owner_->GetCore()->GetConstructionSystem()->GetEmptyData(),
			owner_->GetWorldTransformAdress()->GetWorldPosition(), owner_->GetWorldTransformAdress()->direction_);
		nearKey_ = nearPoint.first;
		AttachVisualizer* ptr = static_cast<AttachVisualizer*>(attachInteract_.get());
		ptr->SetUp(nearPoint.second, owner_->GetCore()->GetWorldTransformAdress()->direction_);
		ptr->Update(nearKey_);
	}
	else {
		// ピックアップ場所表示
		PickupVisualizer* pickUp = static_cast<PickupVisualizer*>(pickupInteract_.get());
		// 検索処理
		// 一番近いの検索→Stateが通常かつ歩いている場合→場所取得（解除・拾う条件式を同じく行う
		MeshObject* interactObject = judgeSystem_->GetNearObject(partsManager_, pickupPointManager_);
		if (interactObject) {
			pickUp->SetTransform(interactObject->GetWorldTransformAdress());
		}
		else {
			pickUp->SetTransform(nullptr);
		}
	}
}

void PlayerPickupManager::ImGuiDraw()
{
	ImGui::Text(nearPartsName_.c_str());

	if (ImGui::Button("FindNearPart")) {
		Car::IParts* nearParts = partsManager_->FindNearParts(owner_->GetWorldTransformAdress()->GetWorldPosition());
		if (nearParts) {
			const Vector3 direct = nearParts->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
			if (owner_->GetFrontChecker()->FrontCheck(direct)) {
				nearPartsName_ = "Front";
			}
			else {
				nearPartsName_ = "NotFront";
			}
		}
		else {
			nearPartsName_ = "None";
		}
	}
	if (ImGui::Button("CatchNearParts")) {
		Car::IParts* nearParts = partsManager_->FindNearParts(owner_->GetWorldTransformAdress()->GetWorldPosition());
		if (nearParts) {
			nearPartsName_ = nearParts->GetName();
			holdParts_ = nearParts;
			//const Vector3 local = holdParts_->GetWorldTransformAdress()->GetWorldPosition() - player_->GetWorldTransformAdress()->GetWorldPosition();
			const Vector3 localOffset = Vector3(0.0f, 0.0f, 2.0f);
			holdParts_->GetWorldTransformAdress()->SetParent(owner_->GetWorldTransformAdress());
			holdParts_->GetWorldTransformAdress()->transform_.translate = localOffset;
			holdParts_->GetWorldTransformAdress()->transform_.rotate = {};
		}
		else {
			nearPartsName_ = "None";
		}
	}
	if (ImGui::Button("ReleaseParts")) {
		nearPartsName_ = "None";
		// ローカル座標→ワールド座標に変換
		//const Vector3 parentRotate = holdParts_->GetWorldTransformAdress()->parent_->transform_.rotate;
		holdParts_->GetWorldTransformAdress()->transform_ = TransformHelper::DetachWithWorldTransform(holdParts_->GetWorldTransformAdress());
		holdParts_->GetWorldTransformAdress()->SetParent(nullptr);
		// パーツの管理を削除
		holdParts_ = nullptr;
	}
}

void PlayerPickupManager::DetachUpdate()
{
}

void PlayerPickupManager::SpotSetup(const std::vector<std::pair<std::string, InteractionSpot*>>& spots)
{
	AttachVisualizer* ptr = static_cast<AttachVisualizer*>(attachInteract_.get());
	// 初期化
	for (auto it = spots.begin(); it != spots.end(); ++it) {
		ptr->AddSpot((*it).first, (*it).second);
	}
}

void PlayerPickupManager::InteractSetup(InteractionSpot* spot)
{
	PickupVisualizer* ptr = static_cast<PickupVisualizer*>(pickupInteract_.get());
	// 設定
	ptr->SetSpot(spot);
}

void PlayerPickupManager::InteractParts()
{
	// 連打回避のタイマー
	if (interactDuration_.has_value()) {
		return;
	}

	// 放す
	if (holdParts_) {
		ReleaseAction();
	}
	// 掴む
	else {
		// ここに今後アルゴリズムを追加する
		CatchAction();
	}

	// インタラクトキーを連打できないように
	const float duration = 15.0f;
	interactDuration_ = FrameTimer(duration);
}

void PlayerPickupManager::ReleaseAction()
{
	// その場に置くかを判断（true:置くため終了,false:置かない為くっつける処理に移行）
	if (ShouldDropPart()) {
		// 置く処理
		DropPart();
		return;
	}
	if (!owner_->GetCore()) {
		DropPart();
		return;
	}
	if (!owner_->GetCore()->GetConstructionSystem()->IsAttach(holdParts_, nearKey_)) {
		DropPart();
		return;
	}
	//// くっつけられるか（false:できなかったためその場に置く
	//if (!holdParts_->SettingParent(partsManager_)) {
	//	DropPart();
	//	return;
	//}

	// SettingParent関数が成功した場合の終了処理
	holdParts_->GetWorldTransformAdress()->transform_.rotate = {};
	holdParts_ = nullptr;
	// インタラクション初期化
	AttachVisualizer* visualizer = static_cast<AttachVisualizer*>(attachInteract_.get());
	visualizer->Reset();
}

void PlayerPickupManager::CatchAction()
{
	// 検索処理
	Car::IParts* interactPart = judgeSystem_->GetCatchPart(partsManager_, pickupPointManager_);

	// パーツが見つかれば
	if (interactPart) {
		OnPartCatchSuccess(interactPart);
	}

}

void PlayerPickupManager::OnPartCatchSuccess(Car::IParts* parts)
{
	// つかみパーツとしてポインタ取得
	holdParts_ = parts;
	// オフセットの位置に設定・親子設定
	const Vector3 localOffset = Vector3(0.0f, 0.0f, 2.0f);
	holdParts_->GetWorldTransformAdress()->SetParent(owner_->GetWorldTransformAdress());
	holdParts_->GetWorldTransformAdress()->transform_.translate = localOffset;
	holdParts_->GetWorldTransformAdress()->transform_.rotate = {};
	holdParts_->OnDetach();

	// 対象の更新
	AttachVisualizer* visualizer = static_cast<AttachVisualizer*>(attachInteract_.get());
	visualizer->RefrashSpot(holdParts_->GetClassNameString());
}

void PlayerPickupManager::OnCatchFailure()
{

}

void PlayerPickupManager::DropPart()
{
	// パーツの位置再設定
	holdParts_->GetWorldTransformAdress()->transform_ = TransformHelper::DetachWithWorldTransform(holdParts_->GetWorldTransformAdress());
	holdParts_->GetWorldTransformAdress()->SetParent(nullptr);
	// コアならスキップ
	if (holdParts_->GetClassNameString() == "VehicleCore") {
		holdParts_ = nullptr;
		return;
	}
	// 所持パーツから解除
	holdParts_ = nullptr;

	// インタラクション初期化
	AttachVisualizer* visualizer = static_cast<AttachVisualizer*>(attachInteract_.get());
	visualizer->Reset();
}

bool PlayerPickupManager::ShouldDropPart()
{
	// クラス名がコアだった場合
	if (holdParts_->GetClassNameString() == "VehicleCore") {
		return true;
	}

	// コアがなければ
	if (!owner_->GetCore()) {
		return true;
	}

	// 距離取得
	float toDistance = TransformHelper::Vector3Distance(owner_->GetWorldTransformAdress()->GetWorldPosition(), owner_->GetCoreTransform()->GetWorldPosition());
	const float limit = 50.0f;
	// 仮置きで距離が一定以内ならくっつけられないように
	if (toDistance > limit) {
		return true;
	}

	// 一番近いキーが無かったら（下の処理の代理）
	if (nearKey_ == Vector2Int(0, 0)) {
		return true;
	}

	//// 一番近いパーツに向いていなければ
	//Car::IParts* parts = partsManager_->FindRootCoreParts(owner_->GetWorldTransformAdress()->GetWorldPosition(), holdParts_);
	//Vector3 playerToNearPartsDirect = parts->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
	//playerToNearPartsDirect.y = 0.0f;
	//playerToNearPartsDirect = Vector3::Normalize(playerToNearPartsDirect);
	//if (!owner_->GetFrontChecker()->FrontCheck(playerToNearPartsDirect)) {
	//	return true;
	//}

	return false;
}
