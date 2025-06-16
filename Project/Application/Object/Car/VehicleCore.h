#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Animation/Animation.h"

#include "Parts/PartsInterface.h"
#include "System/VehicleSystems.h"

class Player;

/// <summary>
/// 車両のコアクラス
/// </summary>
class VehicleCore : public Car::IParts
{
public: // メンバ関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    VehicleCore();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~VehicleCore() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="camera"></param>
    void Draw(BaseCamera& camera) override;

    /// <summary>
    /// ImGuiパーツ用
    /// </summary>
    void ImGuiDrawParts() override;

public: // アクセッサ
    //---ゲッター---//
    // パーツ構築システム
    VehicleConstructionSystem* GetConstructionSystem() { return constructionSystem_.get(); }
    // 運転システム
    DriveSystem* GetDriveSystem() { return driveSystem_.get(); }
    VehicleAnimation* GetAnimation() { return animation_.get(); }
    VehicleStatus* GetStatus() { return statusSystem_.get(); }
    //---セッター---//
    // プレイヤー
    void SetPlayer(Player* player) { pairPlayer_ = player; }

private:
    // パーツ構築システム
    std::unique_ptr<VehicleConstructionSystem> constructionSystem_;
    // 運転関係
    std::unique_ptr<DriveSystem> driveSystem_;
    // 車両のステータス
    std::unique_ptr<VehicleStatus> statusSystem_;
    // アニメーション
    std::unique_ptr<VehicleAnimation> animation_;
    // オーバーヒート
    std::unique_ptr<OverheatSystem> overheatSystem_;

    // ペアになるプレイヤーポインタ
    Player* pairPlayer_ = nullptr;

    Vector3 preDirection_ = {};
};