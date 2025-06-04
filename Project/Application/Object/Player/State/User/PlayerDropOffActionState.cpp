#include "PlayerDropOffActionState.h"
#include "../../Player.h"
#include "../../../Car/VehicleCore.h"
#include "../../../Utility/Calc/MotionHelper.h"


void PlayerDropOffActionState::Initialize()
{
	// アニメーション開始
	float frame = GlobalVariables::GetInstance()->GetFloatValue("Player", "DropOffActionFrame");
	animationTimer_.Start(frame);
	// 開始点設定
	easePoint_.first = player_->GetWorldTransformAdress()->GetWorldPosition();
	Vector3 offset = GlobalVariables::GetInstance()->GetVector3Value("Player", "DropOffOffset");
	// コアのベクトルから
	Vector3 rotateOffset = Matrix4x4::TransformNormal(offset, Matrix4x4::DirectionToDirection(Vector3(0.0f,0.0f,1.0f),player_->GetCoreTransform()->direction_));
	easePoint_.second = player_->GetCoreTransform()->GetWorldPosition() + rotateOffset;

	player_->GetCameraManager()->SetRequest(ActiveCamera::kOverhead);
	player_->GetCore()->GetAnimation()->AnimationStart();
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
