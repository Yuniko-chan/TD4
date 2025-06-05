#include "PlayerPickupManager.h"

#include "../Player.h"
#include "../../Car/CarLists.h"
#include "../../Car/Manager/VehiclePartsManager.h"
#include "../../Car/Manager/PickupPointManager.h"
#include "../../Car/PickupPoint/InterfacePickupPoint.h"
#include "../../Utility/Calc/TransformHelper.h"

#include "../../../Engine/2D/ImguiManager.h"

PlayerPickupManager::PlayerPickupManager()
{
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
		CatchAction();
	}

	// インタラクトキーを連打できないように
	const float duration = 15.0f;
	interactDuration_ = FrameTimer(duration);
}

void PlayerPickupManager::ReleaseAction()
{
	// クラス名がコアだった場合
	if (holdParts_->GetClassNameString() == "VehicleCore") {
		holdParts_->GetWorldTransformAdress()->transform_ = TransformHelper::DetachWithWorldTransform(holdParts_->GetWorldTransformAdress());
		holdParts_->GetWorldTransformAdress()->SetParent(nullptr);
		holdParts_ = nullptr;
		return;
	}

	// コアがなければ
	if (!owner_->GetCore()) {
		holdParts_->GetWorldTransformAdress()->transform_ = TransformHelper::DetachWithWorldTransform(holdParts_->GetWorldTransformAdress());
		holdParts_->GetWorldTransformAdress()->SetParent(nullptr);
		holdParts_ = nullptr;
		return;
	}

	// 距離取得
	float toDistance = TransformHelper::Vector3Distance(owner_->GetWorldTransformAdress()->GetWorldPosition(), owner_->GetCoreTransform()->GetWorldPosition());
	const float limit = 50.0f;
	// 仮置きで距離が一定以内ならくっつけられないように
	if (toDistance > limit) {
		holdParts_->GetWorldTransformAdress()->transform_ = TransformHelper::DetachWithWorldTransform(holdParts_->GetWorldTransformAdress());
		holdParts_->GetWorldTransformAdress()->SetParent(nullptr);
		holdParts_ = nullptr;
		return;
	}

	// 親の設定が上手く行かなかった場合
	if (!holdParts_->SettingParent(partsManager_)) {
		// パーツの位置再設定
		holdParts_->GetWorldTransformAdress()->transform_ = TransformHelper::DetachWithWorldTransform(holdParts_->GetWorldTransformAdress());
		holdParts_->GetWorldTransformAdress()->SetParent(nullptr);
		// パーツの管理を削除
		holdParts_ = nullptr;
		return;
	}

	// SettingParent関数が成功した場合の終了処理
	holdParts_->GetWorldTransformAdress()->transform_.rotate = {};
	holdParts_ = nullptr;

}

void PlayerPickupManager::CatchAction()
{
	//// falseなら受付失敗
	//if (!pickupPointManager_->IsAccept(owner_->GetWorldTransformAdress()->GetWorldPosition())) {
	//	return;
	//}
	Vector3 worldPosition = owner_->GetWorldTransformAdress()->GetWorldPosition();
	// 一番近いポイント（生成箇所）
	IPickupPoint* nearPoint = pickupPointManager_->FindNearPoint(worldPosition);
	// 一番近いパーツ
	Car::IParts* nearParts = partsManager_->FindRootNonCoreParts(worldPosition);
	// 自分のコア
	Car::IParts* nearCore = (partsManager_->FindNearCoreParts(worldPosition));
	bool isEmpty = static_cast<VehicleCore*>(nearCore)->GetConstructionSystem()->IsEmpty();

	// コアに子がない場合
	if ((isEmpty && nearCore) && nearParts && nearPoint) {
		// 方向
		const Vector3 toCoreDirect = nearCore->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
		// 前方チェック
		bool isFrontCore = owner_->GetFrontChecker()->FrontCheck(toCoreDirect);
		float toCore = TransformHelper::Vector3Distance(owner_->GetWorldTransformAdress()->GetWorldPosition(),
			nearCore->GetWorldTransformAdress()->GetWorldPosition());

		// 方向
		const Vector3 toPointDirect = nearPoint->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
		// 前方チェック
		bool isFrontPoint = owner_->GetFrontChecker()->FrontCheck(toPointDirect);
		float toPoint = TransformHelper::Vector3Distance(owner_->GetWorldTransformAdress()->GetWorldPosition(),
			nearPoint->GetWorldTransformAdress()->GetWorldPosition());

		// パーツ
		const Vector3 toPartDirect = nearParts->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
		bool isFrontParts = owner_->GetFrontChecker()->FrontCheck(toPartDirect);
		float toPart = TransformHelper::Vector3Distance(owner_->GetWorldTransformAdress()->GetWorldPosition(),
			nearParts->GetWorldTransformAdress()->GetWorldPosition());

		// ポイントが一番近い
		if ((toPoint < toPart && toPoint < toCore) && isFrontPoint) {
			if (!pickupPointManager_->IsAccept(worldPosition)) {
				return;
			}
			// パーツ取得
			nearParts = pickupPointManager_->AttemptPartAcquisition();
			// 拾う処理
			OnPartCatchSuccess(nearParts);
		}
		// パーツが一番近い
		else if ((toPart < toPoint && toPart < toCore) && isFrontParts) {
			OnPartCatchSuccess(nearParts);
		}
		// コアが一番近い
		else if ((toCore < toPoint && toCore < toPart) && isFrontCore) {
			OnPartCatchSuccess(nearCore);
		}

		//// ポイントの方が近ければ
		//if (toPoint < toPart && isFrontPoint) {
		//	// パーツ取得
		//	nearParts = pickupPointManager_->AttemptPartAcquisition();
		//	// 拾う処理
		//	OnPartCatchSuccess(nearParts);
		//}
		//// パーツの方が近ければ
		//else if (isFrontParts) {
		//	OnPartCatchSuccess(nearParts);
		//}
	}

	// 両方あれば
	else if (nearParts && nearPoint) {
		// 方向
		const Vector3 toPointDirect = nearPoint->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
		// 前方チェック
		bool isFrontPoint = owner_->GetFrontChecker()->FrontCheck(toPointDirect);
		float toPoint = TransformHelper::Vector3Distance(owner_->GetWorldTransformAdress()->GetWorldPosition(),
			nearPoint->GetWorldTransformAdress()->GetWorldPosition());
		
		// パーツ
		const Vector3 toPartDirect = nearParts->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
		bool isFrontParts = owner_->GetFrontChecker()->FrontCheck(toPartDirect);
		float toPart = TransformHelper::Vector3Distance(owner_->GetWorldTransformAdress()->GetWorldPosition(),
			nearParts->GetWorldTransformAdress()->GetWorldPosition());

		// ポイントの方が近ければ
		if (toPoint < toPart && isFrontPoint) {
			if (!pickupPointManager_->IsAccept(worldPosition)) {
				return;
			}
			// パーツ取得
			nearParts = pickupPointManager_->AttemptPartAcquisition();
			// 拾う処理
			OnPartCatchSuccess(nearParts);
		}
		// パーツの方が近ければ
		else if(isFrontParts) {
			OnPartCatchSuccess(nearParts);
		}

	}
	// パーツしかなければ
	else if (nearParts) {
		// 前方チェック
		const Vector3 toPartDirect = nearParts->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
		bool isFrontParts = owner_->GetFrontChecker()->FrontCheck(toPartDirect);
		if (!isFrontParts) {
			return;
		}
		OnPartCatchSuccess(nearParts);
	}
	// ポイントしかなければ
	else if (nearPoint) {
		// 前方チェック
		const Vector3 toPointDirect = nearPoint->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
		bool isFrontPoint = owner_->GetFrontChecker()->FrontCheck(toPointDirect);
		if (!isFrontPoint) {
			return;
		}
		if (!pickupPointManager_->IsAccept(worldPosition)) {
			return;
		}
		nearParts = pickupPointManager_->AttemptPartAcquisition();
		OnPartCatchSuccess(nearParts);
	}

	else if ((isEmpty && nearCore)) {
		// 前方チェック
		const Vector3 toCoreDirect = nearCore->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
		bool isFront = owner_->GetFrontChecker()->FrontCheck(toCoreDirect);
		if (!isFront) {
			return;
		}
		OnPartCatchSuccess(nearCore);
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
}

void PlayerPickupManager::OnCatchFailure()
{

}
