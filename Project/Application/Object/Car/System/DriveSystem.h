#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../Utility/Common/OwnerComponent.h"
#include "../System/VehicleSystems.h"
#include <memory>

class VehicleCore;
class VehicleStatus;

class DriveSystem : public OwnerComponent<VehicleCore>
{
public:
	// コンストラクタ
	DriveSystem();
	// デストラクタ
	~DriveSystem() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	void PreUpdate();
	//void PostUpdate();

	/// <summary>
	/// 入力受付
	/// </summary>
	void InputAccept(GameKeyconfig* keyConfig);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
private:
	void VelocityUpdate();

private:
	// 運転用のエンジン
	std::unique_ptr<VehicleEngine> driveEngine_;
	// ハンドルシステム
	std::unique_ptr<VehicleHandling> handling_;
	// ステータス情報
	VehicleStatus* status_ = nullptr;
	// 速度ベクトル
	Vector3 velocity_ = {};

public:	// アクセッサ
	//---セッター---//
	void SetStatusManager(VehicleStatus* status) { status_ = status; }

	//---ゲッター---//


};
