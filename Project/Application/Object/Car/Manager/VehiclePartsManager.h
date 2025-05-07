#pragma once
#include <unordered_map>
#include <string>
#include "../../../Engine/Math/Vector/Vector3.h"

namespace Car
{
	class IParts;
}

class VehiclePartsManager
{
private:
	/// <summary>
	/// 検索
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	Car::IParts* FindParts(const std::string& tag);

public:
	/// <summary>
	/// 追加処理
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="parts"></param>
	void AddParts(const std::string& tag, Car::IParts* parts);
	/// <summary>
	/// 削除
	/// </summary>
	/// <param name="tag"></param>
	void DeleteParts(const std::string& tag);
	/// <summary>
	/// ImGui表示
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// 一番近いパーツ検索
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	Car::IParts* FindNearParts(const Vector3& position);
	/// <summary>
	/// 一番近いコアパーツ検索
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	Car::IParts* FindNearCoreParts(const Vector3& position);
	/// <summary>
	/// 一番近いコア以外のパーツ検索
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	Car::IParts* FindNearNonCoreParts(const Vector3& position);
private:
	std::unordered_map<std::string, Car::IParts*> partsLists_;
};
