#pragma once
#include <string>
#include <array>
#include "CreateObjectIndex.h"

/// <summary>
/// オブジェクト作成用の名前群
/// </summary>

const std::array<std::string, CreateObjectIndex::kCreateObjectIndexOfCount> kCreateObjectNames_ = {
	"Skydome",
	"Player",
	"VehicleCore",
	"EngineParts",
	"TireParts",
	"ArmorFrameParts",
	"Wall",
	"Course",
	"TerrainObject",
	//ギミック
	"PendulumIronBall",
	"Cannon",
};
