#include "VehicleCaluclator.h"
#include "../../../Engine/Math/Matrix/Matrix4x4.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Car/CarLists.h"

Vector3 VehicleCaluclator::VelocityCalculator()
{
    return Vector3();
}

Vector3 VehicleCaluclator::GetOffset(const Vector2Int& id)
{
    const float kOffset = 2.0f;
    Vector3 result = {};
    result.x = float(id.x) * kOffset;
    result.z = float(id.y) * kOffset;
    return Vector3(result);
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

Vector3 VehicleCaluclator::GetIDToWorldPosition(const Vector2Int& id, WorldTransform* parent)
{
    // IDからのオフセット取得
    Vector3 offset = GetOffset(id);

    // 親がなければ
    if (!parent) {
        return offset;
    }
    // 回転行列から回転
    offset = Matrix4x4::TransformNormal(offset, parent->rotateMatrix_);

    Vector3 result = parent->GetWorldPosition() + offset;

    return Vector3(result);
}

Vector3 VehicleCaluclator::GetEmptyToNearPoint(std::vector<std::pair<Vector2Int, Vector3>>* emptyList, const Vector3& position)
{
    Vector3 result = {};
    float distance = FLT_MAX;
    // 最短距離の物を検索
    for (std::vector<std::pair<Vector2Int, Vector3>>::iterator it = emptyList->begin();
        it != emptyList->end(); ++it) {
        float emptyToPosition = TransformHelper::Vector3Distance((*it).second, position);
        if (distance > emptyToPosition) {
            distance = emptyToPosition;
            result = (*it).second;
        }
    }

    return Vector3(result);
}

Vector3 VehicleCaluclator::GetEmptyToNearPoint(std::vector<std::pair<Vector2Int, Vector3>>* emptyList, const Vector3& position, const Vector3& front)
{
    Vector3 result = {};
    float distance = FLT_MAX;
    // 最短距離の物を検索
    for (std::vector<std::pair<Vector2Int, Vector3>>::iterator it = emptyList->begin();
        it != emptyList->end(); ++it) {
        // 距離
        float emptyToPosition = TransformHelper::Vector3Distance((*it).second, position);
        // 前方チェック
        Vector3 direct = (*it).second - position;
        const float threshold = GlobalVariables::GetInstance()->GetFloatValue("Player", "FrontThreshold");
        bool isfront = FrontCheck(Vector3::Normalize(direct), front, threshold);

        // 前方かつ一番近い
        if (distance > emptyToPosition && isfront) {
            distance = emptyToPosition;
            result = (*it).second;
        }
    }
    // なにもなかった場合
    if (distance == FLT_MAX) {
        return position;
    }

    return Vector3(result);
}

bool VehicleCaluclator::FrontCheck(const Vector3& direct, const Vector3& front, float threshold)
{
    // 対象へのベクトル
    Vector3 newDirect = Vector3(direct.x, 0.0f, direct.z);
    const Vector3 targetDirect = Vector3::Normalize(newDirect);
    // 内積チェック
    float dot = Vector3::Dot(Vector3::Normalize(front), targetDirect);
    // リザルト
    return (dot >= threshold) ? true : false;
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
