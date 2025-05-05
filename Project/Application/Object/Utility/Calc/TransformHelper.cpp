#include "TransformHelper.h"

EulerTransform TransformHelper::DetachWithWorldTransform(const Vector3& parentRotate, WorldTransform* child)
{
    EulerTransform result = {};
    result.scale = child->transform_.scale;
    result.translate = child->GetWorldPosition();
    result.rotate = child->transform_.rotate + parentRotate;
    return EulerTransform(result);
}

EulerTransform TransformHelper::DetachWithWorldTransform(WorldTransform* child)
{
    EulerTransform result = {};
    result.scale = child->transform_.scale;
    result.translate = child->GetWorldPosition();
    result.rotate = child->transform_.rotate + child->parent_->transform_.rotate;
    return EulerTransform(result);
}
