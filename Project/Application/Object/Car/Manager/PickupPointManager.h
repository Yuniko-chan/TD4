#pragma once
#include <unordered_map>
#include <string>
#include "../../../Engine/Math/Vector/Vector3.h"

class IPickupPoint;

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
private:
	// パーツのリスト
	std::unordered_map<std::string, IPickupPoint*> pointLists_;

};
