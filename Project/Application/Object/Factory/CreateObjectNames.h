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
	"InteractionSpot",
	"VehicleCore",
	"EngineParts",
	"TireParts",
	"ArmorFrameParts",
	"EnginePickupPoint",
	"TirePickupPoint",
	"ArmorPickupPoint",
	"CustomArea",
	"Wall",
	"Course",
	"TerrainObject",
	//ギミック
	"PendulumIronBall",
	"Cannon",
	"Minigun",
};
