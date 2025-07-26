#pragma once
#include "../VehicleSystemCommons.h"
#include "../../System/VehicleSystems.h"

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

	void HandleNoParent();
	void BeginSlow();
	void FinishSlow();
private:
	// 運転用のエンジン
	std::unique_ptr<DriveEngine> driveEngine_;
	// ハンドルシステム
	std::unique_ptr<DriveHandling> handling_;
	// ステータス情報
	VehicleStatus* status_ = nullptr;
	// 速度ベクトル
	Vector3 velocity_ = {};
	// ノックバックベクトル
	Vector3 knockBack_ = {};
	// 押し出した回数
	int pushCount_ = 0;
	bool isPush_ = false;
	// 元のベクトル、新しいベクトル
	std::pair<Vector3, Vector3> pushVector_;
	Vector3 pushPower_ = {};
	// スローモーション用のタイマー
	FrameTimer slowTimer_;

	Vector3 totalDirection_ = {};

public:	// アクセッサ
	//---セッター---//
	void SetStatusManager(VehicleStatus* status) { status_ = status; }

	void PushPower(const Vector3& power);

	//---ゲッター---//
	DriveEngine* GetDriveEngine() { return driveEngine_.get(); }

	Vector3 GetVelocity() const { return velocity_; }
	std::pair<Vector3, Vector3> GetPushDirection() const { return pushVector_; }
};
