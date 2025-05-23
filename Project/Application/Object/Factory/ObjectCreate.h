#pragma once
#include "../../../Engine/Level/LevelData.h"
#include "../../../Engine/Object/IObject.h"

// 前方宣言
class Player;

/// <summary>
/// オブジェクト作成関数をまとめたもの
/// </summary>
class ObjectCreate
{

public: //メンバ関数

	/// <summary>
	/// スカイドーム
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectSkydome(LevelData::ObjectData& objectData);

	static IObject* CreateObjectPlayer(LevelData::ObjectData& objectData);

public:	// USER

	static IObject* CreateObjectVehicleCore(LevelData::ObjectData& objectData);

	static IObject* CreateObjectEngineParts(LevelData::ObjectData& objectData);

	static IObject* CreateObjectTireParts(LevelData::ObjectData& objectData);

	static IObject* CreateObjectArmorFrameParts(LevelData::ObjectData& objectData);

	static IObject* CreateObjectIParts(LevelData::ObjectData& objectData);

	static IObject* CreateObjectTerrain(LevelData::ObjectData& objectData);
};
