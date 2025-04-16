#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Animation/Animation.h"

class CarCore : public MeshObject
{
public: // メンバ関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    CarCore();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~CarCore() override;

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

    ///// <summary>
    ///// ImGui描画
    ///// </summary>
    //void ImGuiDraw() override;

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



};