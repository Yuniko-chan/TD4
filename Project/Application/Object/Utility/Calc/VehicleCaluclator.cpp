#include "VehicleCaluclator.h"
#include "../../../Engine/Math/Matrix/Matrix4x4.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../Car/CarLists.h"

Vector3 VehicleCaluclator::VelocityCalculator()
{
    return Vector3();
}

Vector3 VehicleCaluclator::SnapToZero(const Vector3& vector, const float epsilon)
{
    Vector3 result = vector;
    if (std::fabsf(result.x) < epsilon) result.x = 0.0f;
    if (std::fabsf(result.y) < epsilon) result.y = 0.0f;
    if (std::fabsf(result.z) < epsilon) result.z = 0.0f;
    return Vector3(result);
}

Vector3 VehicleCaluclator::RotateVector(const Vector3& velocity, const float euler)
{
    return Vector3(Matrix4x4::TransformNormal(velocity, Matrix4x4::MakeRotateYMatrix(euler)));
}

Vector3 VehicleCaluclator::ExponentialInterpolate(const Vector3& current, const Vector3& end, float decay)
{
    float factor = (1.0f - std::exp(-decay * kDeltaTime_));
    Vector3 v1 = current;
    Vector3 v2 = end;
    return v1 + (v2 - v1) * factor;
}

std::vector<Vector2Int> VehicleCaluclator::GetEmptyList(std::map<Vector2Int, Car::IParts*>* partLists)
{
    std::vector<Vector2Int> parts = {};
    // 全ての隣接パーツをリストに（被りが起きないように
    for (std::map<Vector2Int, Car::IParts*>::iterator it = partLists->begin();
        it != partLists->end(); ++it) {
        Vector2Int key = (*it).first;
        // 四方向のキーを追加する（被りが起きないように）
        Vector2Int findKey = Vector2Int(key.x, key.y + 1);
        parts = PushLists(parts, findKey);
        
        findKey = Vector2Int(key.x, key.y - 1);
        parts = PushLists(parts, findKey);

        findKey = Vector2Int(key.x + 1, key.y);
        parts = PushLists(parts, findKey);
        
        findKey = Vector2Int(key.x - 1, key.y);
        parts = PushLists(parts, findKey);
    }

    // パーツで既にある場合削除
    for (std::vector<Vector2Int>::iterator it = parts.begin();
        it != parts.end();) {
        if (partLists->contains((*it))) {
            it = parts.erase(it);
        }
        else {
            it++;
        }
    }


    return std::vector<Vector2Int>(parts);
}

std::vector<Vector2Int> VehicleCaluclator::PushLists(std::vector<Vector2Int> list, Vector2Int key)
{
    std::vector<Vector2Int> result = list;
    // 追加できるか
    if (std::find(list.begin(), list.end(), key) == list.end() && key != Vector2Int(0, 0)) {
        result.push_back(key);
    }
    return std::vector<Vector2Int>(result);
}
