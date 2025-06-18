#include "VehiclePartsManager.h"
#include "../../../Engine/2D/ImguiManager.h"

#include "../CarLists.h"
#include <cassert>
#include <array>

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
    std::vector<Car::IParts*> tires;
    std::array<std::vector<Car::IParts*>, 4> parts;
    // クラス名
    const std::array<std::string, 4> classNames = {
        "VehicleCore",
        "EngineParts",
        "ArmorFrameParts",
        "TireParts",
    };
    
    // パーツごとにリスト化
    for (std::unordered_map<std::string, Car::IParts*>::iterator it = partsLists_.begin(); it != partsLists_.end(); ++it) {
        if ((*it).second->GetClassNameString() == classNames[0]) {
            parts[0].push_back((*it).second);
        }
        else if ((*it).second->GetClassNameString() == classNames[1]) {
            parts[1].push_back((*it).second);
        }
        else if ((*it).second->GetClassNameString() == classNames[2]) {
            parts[2].push_back((*it).second);
        }
        else if ((*it).second->GetClassNameString() == classNames[3]) {
            parts[3].push_back((*it).second);
        }
    }
    // コア
    for (std::vector<Car::IParts*>::iterator it = parts[0].begin(); it != parts[0].end(); ++it) {
        (*it)->ImGuiDrawParts();
    }

    // コア以外の全てImGui
    if (ImGui::TreeNode("Parts")) {
        for (int i = 1; i < 4; ++i) {
            ImGui::SeparatorText(classNames[i].c_str());
            std::string name = classNames[i] + "Block";
            ImGui::BeginChild(name.c_str(), ImVec2(400, 200), true);

            for (std::vector<Car::IParts*>::iterator it = parts[i].begin(); it != parts[i].end(); ++it) {
                (*it)->ImGuiDrawParts();
            }

            ImGui::EndChild();
        }
        ImGui::TreePop();
    }
    ImGui::End();
}

Car::IParts* VehiclePartsManager::FindNearParts(const Vector3& position)
{
    // 最大設定
    float maxValue = FLT_MAX;
    Car::IParts* targetParts = nullptr;
    for (std::unordered_map<std::string, Car::IParts*>::iterator it = partsLists_.begin(); it != partsLists_.end(); ++it) {
        if (!(*it).second->IsParent()) {
            // 距離ベクトル
            Vector3 distance = (*it).second->GetWorldTransformAdress()->GetWorldPosition() - position;
            float distanceValue = float(std::sqrtf(std::powf(distance.x, 2) + std::powf(distance.y, 2) + std::powf(distance.z, 2)));
            // 距離の値
            if (maxValue > distanceValue) {
                maxValue = distanceValue;
                targetParts = (*it).second;
            }
        }
    }

    return targetParts;
}

Car::IParts* VehiclePartsManager::FindNearCoreParts(const Vector3& position)
{
    // 最大設定
    float maxValue = FLT_MAX;
    Car::IParts* targetParts = nullptr;
    for (std::unordered_map<std::string, Car::IParts*>::iterator it = partsLists_.begin(); it != partsLists_.end(); ++it) {
        // コアじゃなかったらスキップ
        if ((*it).second->GetClassNameString() != "VehicleCore") {
            continue;
        }
        // 距離ベクトル
        Vector3 distance = (*it).second->GetWorldTransformAdress()->GetWorldPosition() - position;
        float distanceValue = float(std::sqrtf(std::powf(distance.x, 2) + std::powf(distance.y, 2) + std::powf(distance.z, 2)));
        // 距離の値
        if (maxValue > distanceValue) {
            maxValue = distanceValue;
            targetParts = (*it).second;
        }
    }

    return targetParts;
}

Car::IParts* VehiclePartsManager::FindNearNonCoreParts(const Vector3& position)
{
    // 最大設定
    float maxValue = FLT_MAX;
    Car::IParts* targetParts = nullptr;
    for (std::unordered_map<std::string, Car::IParts*>::iterator it = partsLists_.begin(); it != partsLists_.end(); ++it) {
        // コアだったらスキップ
        if ((*it).second->GetClassNameString() == "VehicleCore") {
            continue;
        }
        // 距離ベクトル
        Vector3 distance = (*it).second->GetWorldTransformAdress()->GetWorldPosition() - position;
        float distanceValue = float(std::sqrtf(std::powf(distance.x, 2) + std::powf(distance.y, 2) + std::powf(distance.z, 2)));
        // 距離の値
        if (maxValue > distanceValue) {
            maxValue = distanceValue;
            targetParts = (*it).second;
        }
    }

    return targetParts;
}

Car::IParts* VehiclePartsManager::FindRootNonCoreParts(const Vector3& position)
{
    // 最大設定
    float maxValue = FLT_MAX;
    Car::IParts* targetParts = nullptr;
    for (std::unordered_map<std::string, Car::IParts*>::iterator it = partsLists_.begin(); it != partsLists_.end(); ++it) {
        // コアだったらスキップ
        if ((*it).second->GetClassNameString() == "VehicleCore") {
            continue;
        }
        // 親がいればスキップ
        if ((*it).second->IsParent()) {
            continue;
        }
        // 距離ベクトル
        Vector3 distance = (*it).second->GetWorldTransformAdress()->GetWorldPosition() - position;
        float distanceValue = float(std::sqrtf(std::powf(distance.x, 2) + std::powf(distance.y, 2) + std::powf(distance.z, 2)));
        // 距離の値
        if (maxValue > distanceValue) {
            maxValue = distanceValue;
            targetParts = (*it).second;
        }
    }

    return targetParts;
}

Car::IParts* VehiclePartsManager::FindRootCoreParts(const Vector3& position)
{
    // 最大設定
    float maxValue = FLT_MAX;
    Car::IParts* targetParts = nullptr;
    for (std::unordered_map<std::string, Car::IParts*>::iterator it = partsLists_.begin(); it != partsLists_.end(); ++it) {
        // コアだったらスキップ
        if ((*it).second->GetClassNameString() == "VehicleCore") {
            continue;
        }
        // 親がいればスキップ
        if (!(*it).second->IsParent()) {
            continue;
        }
        // 距離ベクトル
        Vector3 distance = (*it).second->GetWorldTransformAdress()->GetWorldPosition() - position;
        float distanceValue = float(std::sqrtf(std::powf(distance.x, 2) + std::powf(distance.y, 2) + std::powf(distance.z, 2)));
        // 距離の値
        if (maxValue > distanceValue) {
            maxValue = distanceValue;
            targetParts = (*it).second;
        }
    }

    return targetParts;
}
