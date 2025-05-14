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

float TransformHelper::Vector3Distance(const Vector3& v1, const Vector3& v2)
{
    Vector3 t1 = v1;    // 一個目の座標
    Vector3 t2 = v2;    // 二個目の座標
    Vector3 distance = t1 - t2;     // ベクトル計算
    // 距離の値
    float distanceValue = (std::sqrtf(std::powf(distance.x, 2)) + std::sqrtf(std::powf(distance.y, 2)) + std::sqrtf(std::powf(distance.z, 2)));
    return distanceValue;
}
