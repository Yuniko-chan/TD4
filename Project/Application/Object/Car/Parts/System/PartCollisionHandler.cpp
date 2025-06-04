#include "PartCollisionHandler.h"
#include "../../../GameObjectsList.h"
#include "../../Parts/PartsInterface.h"

// 通常ダメ―ジ
int16_t PartCollisionHandler::usuallyDamage_ = 1;
// ミニガンダメ―ジ
int16_t PartCollisionHandler::MinigunDamage_ = 1;

void PartCollisionHandler::Initialize()
{
    // ダメ―ジ
    usuallyDamage_ = 1;
    MinigunDamage_ = 1;
}

void PartCollisionHandler::Execute(Car::IParts* part, ColliderParentObject colliderPartner)
{

    // ギミックオブジェクト取得
    GimmickObject gimmickObject = CheckGimmick(colliderPartner);

    if (!std::holds_alternative<Obstacle*>(gimmickObject)) {
        OnCollisionGimmick(part, gimmickObject);
    }
    else {
        // null確認
        Obstacle* obstacle = std::get<Obstacle*>(gimmickObject);
        if (obstacle) {
            OnCollisionGimmick(part, gimmickObject);
        }
    }

}

PartCollisionHandler::GimmickObject PartCollisionHandler::CheckGimmick(ColliderParentObject colliderPartner)
{

    // null
    GimmickObject gimmickObject = static_cast<Obstacle*>(nullptr);

    // ConicalPendulumIronBall*
    if (std::holds_alternative<ConicalPendulumIronBall*>(colliderPartner)) {
        gimmickObject = std::get<ConicalPendulumIronBall*>(colliderPartner);
    }
    // PendulumIronBall*
    else if (std::holds_alternative<PendulumIronBall*>(colliderPartner)) {
        gimmickObject = std::get<PendulumIronBall*>(colliderPartner);
    }
    // CannonBall*
    else if (std::holds_alternative<CannonBall*>(colliderPartner)) {
        gimmickObject = std::get<CannonBall*>(colliderPartner);
    }
    // CannonExplosion*
    else if (std::holds_alternative<CannonExplosion*>(colliderPartner)) {
        gimmickObject = std::get<CannonExplosion*>(colliderPartner);
    }
    // MinigunBullet*
    else if (std::holds_alternative<MinigunBullet*>(colliderPartner)) {
        gimmickObject = std::get<MinigunBullet*>(colliderPartner);
    }
    // Obstacle*
    else if (std::holds_alternative<Obstacle*>(colliderPartner)) {
        gimmickObject = std::get<Obstacle*>(colliderPartner);
    }

    return gimmickObject;

}

void PartCollisionHandler::OnCollisionGimmick(Car::IParts* part, GimmickObject colliderPartner)
{

    // ダメージ取得
    int16_t damage = usuallyDamage_;
    if (std::holds_alternative<MinigunBullet*>(colliderPartner)) {
        damage = MinigunDamage_;
    }

    part->GetHPHandler()->OnHit(damage);

}
