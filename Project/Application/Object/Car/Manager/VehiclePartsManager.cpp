#include "VehiclePartsManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include <cassert>

void VehiclePartsManager::AddParts(const std::string& tag, Car::IParts* parts)
{
    Car::IParts* partsPointer = FindParts(tag);
    if (!partsPointer) {
        partsLists_.emplace(tag, parts);
    }
}

Car::IParts* VehiclePartsManager::FindParts(const std::string& tag)
{
    // イテレータ
    std::unordered_map<std::string, Car::IParts*>::iterator it = partsLists_.find(tag);

    if (it == partsLists_.end()) {
        return nullptr;
    }

    return (*it).second;
    //// チェック
    //assert(it != partsLists_.end());
    //// 返し
    //return (*it).second;
}

void VehiclePartsManager::DeleteParts(const std::string& tag)
{
    // イテレータ
    std::unordered_map<std::string, Car::IParts*>::iterator it = partsLists_.find(tag);
    // チェック
    assert(it != partsLists_.end());
    // 削除
    partsLists_.erase(it);
}

void VehiclePartsManager::ImGuiDraw()
{
    ImGui::Begin("PartsManager");

    ImGui::End();
}
