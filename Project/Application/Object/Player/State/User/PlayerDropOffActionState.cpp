#include "PlayerDropOffActionState.h"
#include "../../Player.h"
#include "../../../Utility/Calc/MotionHelper.h"


void PlayerDropOffActionState::Initialize()
{
	// アニメーション開始
	float frame = GlobalVariables::GetInstance()->GetFloatValue("Player", "DropOffActionFrame");
	animationTimer_.Start(frame);
	//player_->GetCamera()->ChangeRequest(FollowCamera::AngleMode::kPlayer, frame);
	// 開始点設定
	easePoint_.first = player_->GetWorldTransformAdress()->GetWorldPosition();
	Vector3 offset = GlobalVariables::GetInstance()->GetVector3Value("Player", "DropOffOffset");
	Vector3 rotateOffset = Matrix4x4::TransformNormal(offset, Matrix4x4::MakeRotateYMatrix(player_->GetCoreTransform()->transform_.rotate.y));
	easePoint_.second = player_->GetCoreTransform()->GetWorldPosition() + rotateOffset;

	player_->GetCameraManager()->SetRequest(ActiveCamera::kOverhead);
}

void PlayerDropOffActionState::Update()
{
	// タイマー
	animationTimer_.Update();
	// アクティブ
	if (animationTimer_.IsActive()) {
		float jumpHeight = GlobalVariables::GetInstance()->GetFloatValue("Player", "DropOffHeight");
		player_->GetWorldTransformAdress()->transform_.translate = MotionHelper::CurveBezier(easePoint_.first, easePoint_.second,
			animationTimer_.GetElapsedFrame(), jumpHeight);
	}
	// 終了
	if (animationTimer_.IsEnd()) {
		player_->GetStateMachine()->ChangeRequest(IPlayerState::kOnFoot);
	}
}

void PlayerDropOffActionState::Exit()
{

}
