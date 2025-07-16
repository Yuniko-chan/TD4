#include "CustomArea.h"
#include "../../Collider/CollisionConfig.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../Course/CourseManager.h"
#include "../Car/VehicleCore.h"


CustomArea::CustomArea()
{
}

CustomArea::~CustomArea()
{
}

void CustomArea::Initialize(LevelData::MeshData* data)
{
#ifdef _DEBUG
    isVisible_ = true;
#endif // _DEBUG


    // 初期化
    MeshObject::Initialize(data);

}


void CustomArea::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
    colliderPartner, collisionData;
    if (!isTouchPlayer_ && std::holds_alternative<VehicleCore*>(colliderPartner)) {
        //チェックポイント通過処理
        isTouchPlayer_ = true;
        courseManager_->AddCourse();
        if (gimmickList_) {
            for (auto it : *gimmickList_) {
                it->SetIsDead(true);
            }
        }
    }
}

void CustomArea::CollisionListRegister(BaseCollisionManager* collisionManager)
{

    // 衝突登録
    MeshObject::CollisionListRegister(collisionManager);


}

void CustomArea::CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

    // 衝突登録
    MeshObject::CollisionListRegister(collisionManager, colliderDebugDraw);


}

void CustomArea::ColliderInitialize(ColliderShape collider)
{

    // 衝突マスク
    collisionAttribute_ = kColisionAttributeGimmick;
    collisionMask_ -= kColisionAttributeGimmick;

    // コライダー
    OBB obb = std::get<OBB>(collider);
    obb.SetParentObject(this);
    obb.SetCollisionAttribute(collisionAttribute_);
    obb.SetCollisionMask(collisionMask_);
    ColliderShape* colliderShape = new ColliderShape();
    *colliderShape = obb;
    collider_.reset(colliderShape);

}

