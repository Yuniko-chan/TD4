#include "PlayerPickupManager.h"

#include "../Player.h"
#include "../../Car/Parts/PartsInterface.h"
#include "../../Car/Manager/VehiclePartsManager.h"
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

	// パーツの位置再設定
	holdParts_->GetWorldTransformAdress()->transform_ = TransformHelper::DetachWithWorldTransform(holdParts_->GetWorldTransformAdress());
	holdParts_->GetWorldTransformAdress()->transform_.translate = {};
	holdParts_->GetWorldTransformAdress()->transform_.rotate = {};
	holdParts_->GetWorldTransformAdress()->SetParent(nullptr);

	// 近くのコアにセット
	holdParts_->SettingParent(partsManager_);

	// パーツの管理を削除
	holdParts_ = nullptr;
}

void PlayerPickupManager::CatchAction()
{
	// 一番近いパーツ
	Car::IParts* nearParts = partsManager_->FindRootNonCoreParts(owner_->GetWorldTransformAdress()->GetWorldPosition());
	// エラー回避用
	if (nearParts) {
		// 失敗
		OnCatchFailure();

		// 成功
		OnCatchSuccess(nearParts);
	}
}

void PlayerPickupManager::OnCatchSuccess(Car::IParts* parts)
{
	// 方向
	const Vector3 direct = parts->GetWorldTransformAdress()->GetWorldPosition() - owner_->GetWorldTransformAdress()->GetWorldPosition();
	// 前方の閾値内か
	if (owner_->GetFrontChecker()->FrontCheck(direct)) {
		// 最大距離より外ならつかまない
		if (!owner_->GetFrontChecker()->IsInRange(parts->GetWorldTransformAdress()->GetWorldPosition())) {
			return;
		}
		// つかみパーツとしてポインタ取得
		holdParts_ = parts;
		// オフセットの位置に設定・親子設定
		const Vector3 localOffset = Vector3(0.0f, 0.0f, 2.0f);
		holdParts_->GetWorldTransformAdress()->SetParent(owner_->GetWorldTransformAdress());
		holdParts_->GetWorldTransformAdress()->transform_.translate = localOffset;
		holdParts_->GetWorldTransformAdress()->transform_.rotate = {};
	}

}

void PlayerPickupManager::OnCatchFailure()
{
}
