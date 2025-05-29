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
    // 親がいれば
    if (child->parent_) {
        result.rotate = child->transform_.rotate + child->parent_->transform_.rotate;
    }
    else {
        result.rotate = child->transform_.rotate;
    }
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

Vector3 TransformHelper::XZRotateDirection(const Vector3& direct, float& theta)
{
    float cosT = std::cosf(theta);
    float sinT = std::sinf(theta);

    Vector3 result = {
        direct.x * cosT - direct.z * sinT,
        direct.y,
        direct.x * sinT + direct.z * cosT,
    };

    return Vector3(Vector3::Normalize(result));
}

float TransformHelper::CalculateXZVectorToRotateRadian(const Vector3& from, const Vector3& to)
{
    Vector2 from2 = Vector2(from.x, from.z);
    Vector2 to2 = Vector2(to.x, to.z);
    from2 = Vector2::Normalize(from2);
    to2 = Vector2::Normalize(to2);

    float cos = Vector2::Dot(from2, to2);
    float rad = std::acosf(cos);

    //float angle = rad / (3.14f * 180.0f);

    float cross = Vector2::Cross(from2, to2);

    if (cross < 0) {
        return rad * (-1.0f);
    }

    return rad;
}

Vector3 TransformHelper::XZRotation(const Vector3& direct, float& theta)
{
    // θ
    float cosT = std::cosf(theta);
    float sinT = std::sinf(theta);
    // 回転
    Vector3 result = {
        direct.x * cosT - direct.z * sinT,
        direct.y,
        direct.x * sinT + direct.z * cosT,
    };
    // 返り値
    return result;
}
