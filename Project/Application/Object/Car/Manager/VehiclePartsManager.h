#pragma once
#include <unordered_map>
#include <string>

namespace Car
{
	class IParts;
}

class VehiclePartsManager
{
public:
	/// <summary>
	/// 検索
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	Car::IParts* FindParts(const std::string& tag);
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

private:
	std::unordered_map<std::string, Car::IParts*> partsLists_;
};
