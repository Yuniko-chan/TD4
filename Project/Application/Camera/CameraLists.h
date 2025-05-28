#pragma once
#include "../../../Engine/Camera/BaseCamera.h"
#include "FollowCamera.h"
#include "OverheadCamera.h"

/// <summary>
/// カメラの一覧、アクティブチェック用のやつ
/// </summary>
enum class ActiveCamera
{
	kDebug,	// デバッグ
	kFollow,	// 追従
	kOverhead,	// 俯瞰
	kMaxCount,
};
