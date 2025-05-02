#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Animation/Animation.h"

#include "Parts/PartsInterface.h"
#include "System/VehicleStatus.h"

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

    ///// <summary>
    ///// 描画
    ///// </summary>
    ///// <param name="camera">カメラ</param>
    //void Draw(BaseCamera& camera) override;

    /// <summary>
    /// ImGui描画
    /// </summary>
    void ImGuiDraw() override;

    /// <summary>
    /// 
    /// </summary>
    void ImGuiDrawParts() override;

    ///// <summary>
    ///// パーティクル描画
    ///// </summary>
    ///// <param name="camera">カメラ</param>
    //void ParticleDraw(BaseCamera& camera) override;

    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

    void MoveCommand();

    void SetPlayer(Player* player) { pairPlayer_ = player; }

private:
    // パーツを配置できる四か所（メッシュオブジェクトにするかも）
    std::list<WorldTransform> fourPoints_;
    // パーツのリスト
    std::list<Car::IParts*> partsLists_;

    // ペアになるプレイヤーポインタ
    Player* pairPlayer_ = nullptr;
    // ステータス
    VehicleStatus status_ = {};
};