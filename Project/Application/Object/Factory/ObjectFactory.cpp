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

	// 地形
	createObjectFunctions_[kCreateObjectIndexTerrain].first = kCreateObjectNames_[kCreateObjectIndexTerrain];
	createObjectFunctions_[kCreateObjectIndexTerrain].second = ObjectCreate::CreateObjectTerrain;

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

	return object;

}
