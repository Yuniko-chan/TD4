#include "PartCollisionHandler.h"
#include "../../../GameObjectsList.h"
#include "../../Parts/PartsInterface.h"

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
    int16_t damage = 0;

    std::visit([&](auto x) {
        damage = x->GetDamage();
        }, colliderPartner);

    part->GetHPHandler()->OnHit(damage);

}
