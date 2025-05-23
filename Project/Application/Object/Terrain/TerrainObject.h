#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../BaseCharacter/BaseCharacterAnimation.h"

class TerrainObject : public MeshObject
{
public:
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
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

private:
    std::unique_ptr<BaseCharacterAnimation> animation_;

};
