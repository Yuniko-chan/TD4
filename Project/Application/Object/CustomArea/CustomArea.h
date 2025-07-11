#pragma once
#include "../../../Engine/Object/MeshObject.h"

class CourseManager;

/// <summary>
/// カスタムエリア
/// </summary>
class CustomArea :
    public MeshObject
{

public: // 関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    CustomArea();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~CustomArea() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

  
    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

    /// <summary>
    /// コライダー登録
    /// </summary>
    /// <param name="collisionManager">衝突マネージャー</param>
    void CollisionListRegister(BaseCollisionManager* collisionManager);

    /// <summary>
    /// コライダー登録
    /// </summary>
    /// <param name="collisionManager">衝突マネージャー</param>
    /// <param name="colliderDebugDraw">衝突デバッグ描画</param>
    void CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw);

    void SetCourseManager(CourseManager* manager) { courseManager_ = manager; };

private: // 関数

    /// <summary>
    /// コライダーの初期化
    /// </summary>
    /// <param name="collider">コライダー</param>
    void ColliderInitialize(ColliderShape collider) override;

private: // 変数

    bool isVisible_ = false;

    //プレイヤーが一度でも触れたかどうか
    bool isTouchPlayer_ = false;

    CourseManager* courseManager_ = nullptr;
};

