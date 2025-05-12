#pragma once
#include <string>
#include <array>

enum class ObjectIndex : size_t {
	kPlayer,
	kEngine,
	kFrame,
	kTire,
	kTerrain,
	kCount,
};

/// <summary>
/// 
/// </summary>
struct GameObjectIncludeNames
{
	std::string className;
	std::string filePath;
	std::string modelName;
};

const std::array<GameObjectIncludeNames, (size_t)ObjectIndex::kCount> kModelIncludes = {
	{},
};

//std::string a = kModelIncludes[(size_t)ObjectIndex::kPlayer].className;