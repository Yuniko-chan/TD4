#pragma once
#include "User/ArmorPickupPoint.h"
#include "User/EnginePickupPoint.h"
#include "User/TirePickupPoint.h"

#include "InterfacePickupPoint.h"

/// <summary>
/// ピックアップポイント作成用の名前群
/// </summary>

const size_t kPickupPointCount_ = 3;

const std::array<std::string, kPickupPointCount_> kRegisterPickupPointNames_ = {
	"EnginePickupPoint",
	"TirePickupPoint",
	"ArmorPickupPoint",
};
