#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Utility/Common/OwnerComponent.h"
#include "../../System/VehicleSystems.h"
#include <memory>

class VehicleCore;

class DriveSystem : public OwnerComponent<VehicleCore>
{
public:
	// コンストラクタ
	DriveSystem();
	// デストラクタ
	~DriveSystem() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void RotateUpdate();

	/// <summary>
	/// 入力受付
	/// </summary>
	void InputAccept(GameKeyconfig* keyConfig, const Vector3& direct);

	// 親の設定
	void SetTransform(WorldTransform* core) { coreTransform_ = core; }

	void ImGuiDraw();

private:
	// 運転用のエンジン
	std::unique_ptr<VehicleEngine> driveEngine_;
	// トランスフォーム
	WorldTransform* coreTransform_ = nullptr;

	// 速度ベクトル
	Vector3 velocity_ = {};
};
