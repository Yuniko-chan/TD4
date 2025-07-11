#pragma once
#include "../Math/Vector2Int.h"
#include "../../../Engine/Math/Vector/Vector3.h"
#include "../../../Engine/3D/Transform/WorldTransform.h"

#include <map>
#include <vector>

namespace Car { class IParts; }
class DriveSystem;

/// <summary>
/// マッピングキー、3次元座標
/// </summary>
using MappingKey = std::pair<Vector2Int, Vector3>;

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

	/// <summary>
	/// 近いポイントを配置されていない空のリストから検索（座標取得
	/// </summary>
	/// <param name="emptyList">リスト</param>
	/// <param name="position">元座標</param>
	/// <returns></returns>
	Vector3 GetEmptyToNearPoint(std::vector<MappingKey>* emptyList, const Vector3& position);
	/// <summary>
	/// 近いポイントを配置されていない空のリストから検索（マッピングのセットで取得
	/// </summary>
	/// <param name="emptyList">空のリスト</param>
	/// <param name="position">比較の元座標</param>
	/// <param name="front">前方</param>
	/// <returns></returns>
	MappingKey GetEmptyToNearPoint(std::vector<MappingKey>* emptyList, const Vector3& position, const Vector3& front);
	/// <summary>
	/// 前方チェック
	/// </summary>
	/// <param name="direct">向き</param>
	/// <param name="front">前方ベクトル</param>
	/// <param name="threshold">閾値</param>
	/// <returns></returns>
	bool FrontCheck(const Vector3& direct, const Vector3& front, float threshold);

	float BlurStrengthFactor(DriveSystem* driveSystem, float blurSpeed, float blurMaxSpeed);

private:
	static std::vector<Vector2Int> PushLists(std::vector<Vector2Int> list, Vector2Int key);
};
