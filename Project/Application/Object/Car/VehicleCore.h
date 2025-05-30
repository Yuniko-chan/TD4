#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Animation/Animation.h"

#include "Parts/PartsInterface.h"
#include "System/VehicleSystems.h"
#include "Construction/VehicleConstructionSystem.h"

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
    /// ImGuiパーツ用
    /// </summary>
    void ImGuiDrawParts() override;

    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

    /// <summary>
    /// 子の追加
    /// </summary>
    /// <param name="child"></param>
    void AddChild(Car::IParts* child) { partsLists_.push_back(child); }

public: // アクセッサ
    //---ゲッター---//
    // パーツ構築システム
    VehicleConstructionSystem* GetConstructionSystem() { return constructionSystem_.get(); }
    // 運転システム
    DriveSystem* GetDriveSystem() { return driveSystem_.get(); }

    //---セッター---//
    // プレイヤー
    void SetPlayer(Player* player) { pairPlayer_ = player; }

private:
    // パーツのリスト
    std::list<Car::IParts*> partsLists_;

    // パーツ構築システム
    std::unique_ptr<VehicleConstructionSystem> constructionSystem_;
    // 運転関係
    std::unique_ptr<DriveSystem> driveSystem_;

    // ペアになるプレイヤーポインタ
    Player* pairPlayer_ = nullptr;
    // ステータス
    VehicleStatus status_ = {};
};