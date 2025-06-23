#pragma once
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../Math/Vector2Int.h"
#include "../../../Engine/3D/Transform/WorldTransform.h"

#include <map>
#include <vector>

namespace Car { class IParts; }

class VehicleCaluclator
{
public:
	/// <summary>
	/// 加速度追加計算
	/// </summary>
	/// <returns></returns>
	//float AccelAddCalculator();

	Vector3 VelocityCalculator();
	/// <summary>
	/// 値を0に
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="epsilon"></param>
	/// <returns></returns>
	Vector3 SnapToZero(const Vector3& vector, const float epsilon);

	Vector3 RotateVector(const Vector3& velocity, const float euler);
	/// <summary>
	/// 指数関数補間
	/// </summary>
	/// <param name="current"></param>
	/// <param name="end"></param>
	/// <param name="decay"></param>
	/// <returns></returns>
	static Vector3 ExponentialInterpolate(const Vector3& current, const Vector3& end, float decay);

	static std::vector<Vector2Int> GetEmptyList(std::map<Vector2Int, Car::IParts*>* partLists);

	/// <summary>
	/// オフセット
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	Vector3 GetOffset(const Vector2Int& id);

	/// <summary>
	/// IDから親を含めたワールド座標計算
	/// </summary>
	/// <param name="id"></param>
	/// <param name="parent"></param>
	/// <returns></returns>
	Vector3 GetIDToWorldPosition(const Vector2Int& id, WorldTransform* parent);


	Vector3 GetEmptyToNearPoint(std::vector<std::pair<Vector2Int, Vector3>>* emptyList, const Vector3& position);
	std::pair<Vector2Int, Vector3> GetEmptyToNearPoint(std::vector<std::pair<Vector2Int, Vector3>>* emptyList, const Vector3& position, const Vector3& front);
	bool FrontCheck(const Vector3& direct, const Vector3& front, float threshold);

private:
	static std::vector<Vector2Int> PushLists(std::vector<Vector2Int> list, Vector2Int key);
};
