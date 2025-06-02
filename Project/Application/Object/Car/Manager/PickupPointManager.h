#pragma once
#include <unordered_map>
#include <string>
#include "../../../Engine/Math/Vector/Vector3.h"

namespace Car
{
	class IParts;
}

class IPickupPoint;
class BaseObjectManager;

class PickupPointManager
{
public:
	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="newPoint"></param>
	void AddPickupPoint(const std::string& tag, IPickupPoint* newPoint);
	/// <summary>
	/// 削除
	/// </summary>
	/// <param name="tag"></param>
	void DeletePickupPoint(const std::string& tag);
	/// <summary>
	/// 近いポイント検索
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	IPickupPoint* FindNearPoint(const Vector3& position);

	//void AttemptPartAcquisition(const Vector3& position);
	Car::IParts* AttemptPartAcquisition();

	bool IsAccept(const Vector3& position);

public: // アクセッサ

	void SetObjectManager(BaseObjectManager* objectManager) { objectManager_ = objectManager; }

private:
	Car::IParts* GenerateParts(int32_t partNum);

private:
	// パーツのリスト
	std::unordered_map<std::string, IPickupPoint*> pointLists_;
	// オブジェクトマネージャー
	BaseObjectManager* objectManager_ = nullptr;
	// 拾うやつ
	IPickupPoint* pickupPoint_ = nullptr;
};
