#include "ObjectFactory.h"
#include "../../../Engine/Object/MeshObject.h"
#include "ObjectCreate.h"

#include "CreateObjectNames.h"

// オブジェクト作成でそれぞれのタイプを作成するための関数群
// 返り値 無し
// 引数1 オブジェクト
std::array<
	std::pair<std::string, std::function<IObject*(LevelData::ObjectData&)>>,
	CreateObjectIndex::kCreateObjectIndexOfCount> ObjectFactory::createObjectFunctions_ = {};

// マネージャー
BaseObjectManager* ObjectFactory::objectManager_ = nullptr;

void ObjectFactory::Initialize(BaseObjectManager* objectManager)
{

	objectManager_ = objectManager;

	// 関数を入れていく

	// スカイドーム
	createObjectFunctions_[kCreateObjectIndexSkydome].first = kCreateObjectNames_[kCreateObjectIndexSkydome];
	createObjectFunctions_[kCreateObjectIndexSkydome].second = ObjectCreate::CreateObjectSkydome;

	// プレイヤー
	createObjectFunctions_[kCreateObjectIndexPlayer].first = kCreateObjectNames_[kCreateObjectIndexPlayer];
	createObjectFunctions_[kCreateObjectIndexPlayer].second = ObjectCreate::CreateObjectPlayer;
	
	// インタラクト
	createObjectFunctions_[kCreateObjectIndexInteract].first = kCreateObjectNames_[kCreateObjectIndexInteract];
	createObjectFunctions_[kCreateObjectIndexInteract].second = ObjectCreate::CreateObjectInteract;

	///---USER---///
	// 車両コア
	createObjectFunctions_[kCreateObjectIndexVehicleCore].first = kCreateObjectNames_[kCreateObjectIndexVehicleCore];
	createObjectFunctions_[kCreateObjectIndexVehicleCore].second = ObjectCreate::CreateObjectVehicleCore;
	// エンジンパーツ
	createObjectFunctions_[kCreateObjectIndexCarEngine].first = kCreateObjectNames_[kCreateObjectIndexCarEngine];
	createObjectFunctions_[kCreateObjectIndexCarEngine].second = ObjectCreate::CreateObjectEngineParts;
	// タイヤパーツ
	createObjectFunctions_[kCreateObjectIndexCarTire].first = kCreateObjectNames_[kCreateObjectIndexCarTire];
	createObjectFunctions_[kCreateObjectIndexCarTire].second = ObjectCreate::CreateObjectTireParts;
	// アーマーパーツ
	createObjectFunctions_[kCreateObjectIndexCarArmorFrame].first = kCreateObjectNames_[kCreateObjectIndexCarArmorFrame];
	createObjectFunctions_[kCreateObjectIndexCarArmorFrame].second = ObjectCreate::CreateObjectArmorFrameParts;

	// エンジンポイント
	createObjectFunctions_[kCreateObjectIndexEnginePoint].first = kCreateObjectNames_[kCreateObjectIndexEnginePoint];
	createObjectFunctions_[kCreateObjectIndexEnginePoint].second = ObjectCreate::CreateObjectEnginePoint;
	// タイヤポイント
	createObjectFunctions_[kCreateObjectIndexTirePoint].first = kCreateObjectNames_[kCreateObjectIndexTirePoint];
	createObjectFunctions_[kCreateObjectIndexTirePoint].second = ObjectCreate::CreateObjectTirePoint;
	// アーマーポイント
	createObjectFunctions_[kCreateObjectIndexArmorPoint].first = kCreateObjectNames_[kCreateObjectIndexArmorPoint];
	createObjectFunctions_[kCreateObjectIndexArmorPoint].second = ObjectCreate::CreateObjectArmorPoint;
	//カスタムエリア
	createObjectFunctions_[kCreateObjectIndexCustomArea].first = kCreateObjectNames_[kCreateObjectIndexCustomArea];
	createObjectFunctions_[kCreateObjectIndexCustomArea].second = ObjectCreate::CreateObjectCustomArea;

	//壁
	createObjectFunctions_[kCreateObjectIndexWall].first = kCreateObjectNames_[kCreateObjectIndexWall];
	createObjectFunctions_[kCreateObjectIndexWall].second = ObjectCreate::CreateObjectWall;

	//コース
	createObjectFunctions_[kCreateObjectIndexCourse].first = kCreateObjectNames_[kCreateObjectIndexCourse];
	createObjectFunctions_[kCreateObjectIndexCourse].second = ObjectCreate::CreateObjectCourse;

	// 地形
	createObjectFunctions_[kCreateObjectIndexTerrain].first = kCreateObjectNames_[kCreateObjectIndexTerrain];
	createObjectFunctions_[kCreateObjectIndexTerrain].second = ObjectCreate::CreateObjectTerrain;

	///ギミック
	createObjectFunctions_[kCreateObjectIndexIronBall].first = kCreateObjectNames_[kCreateObjectIndexIronBall];
	createObjectFunctions_[kCreateObjectIndexIronBall].second = ObjectCreate::CreateObjectIronBall;

	createObjectFunctions_[kCreateObjectIndexCannon].first = kCreateObjectNames_[kCreateObjectIndexCannon];
	createObjectFunctions_[kCreateObjectIndexCannon].second = ObjectCreate::CreateObjectCannon;

	createObjectFunctions_[kCreateObjectIndexMinigun].first = kCreateObjectNames_[kCreateObjectIndexMinigun];
	createObjectFunctions_[kCreateObjectIndexMinigun].second = ObjectCreate::CreateObjectMinigun;
}

IObject* ObjectFactory::CreateObject(LevelData::ObjectData& objectData)
{

	IObject* object = nullptr;

	// 確認のためメッシュオブジェクトのみ
	// クラスの名前など取得してオブジェクトを作る
	if (std::holds_alternative<LevelData::MeshData>(objectData)) {

		LevelData::MeshData data = std::get<LevelData::MeshData>(objectData);

		for (uint32_t i = 0; i < kCreateObjectIndexOfCount; ++i) {
			if (data.className == createObjectFunctions_[i].first) {
				object = createObjectFunctions_[i].second(objectData);
			}

		}

	}

	//ギミック
	if (std::holds_alternative<LevelData::GimmickData>(objectData)) {

		LevelData::GimmickData data = std::get<LevelData::GimmickData>(objectData);

		for (uint32_t i = 0; i < kCreateObjectIndexOfCount; ++i) {
			if (data.meshData.className == createObjectFunctions_[i].first) {
				object = createObjectFunctions_[i].second(objectData);
			}

		}

	}

    return object;

}

IObject* ObjectFactory::CreateObjectPattern(LevelData::ObjectData& objectData)
{

	IObject* object = nullptr;

	// 確認のためメッシュオブジェクトのみ
	// クラスの名前など取得してオブジェクトを作る
	if (std::holds_alternative<LevelData::MeshData>(objectData)) {

		LevelData::MeshData data = std::get<LevelData::MeshData>(objectData);

		for (uint32_t i = 0; i < kCreateObjectIndexOfCount; ++i) {
			if (data.className == createObjectFunctions_[i].first) {
				object = createObjectFunctions_[i].second(objectData);
			}

		}

	}

	//ギミック
	if (std::holds_alternative<LevelData::GimmickData>(objectData)) {

		LevelData::GimmickData data = std::get<LevelData::GimmickData>(objectData);

		for (uint32_t i = 0; i < kCreateObjectIndexOfCount; ++i) {
			if (data.meshData.className == createObjectFunctions_[i].first) {
				object = createObjectFunctions_[i].second(objectData);
			}

		}

	}


	return object;

}
