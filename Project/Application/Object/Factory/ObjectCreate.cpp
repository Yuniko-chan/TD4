#include "ObjectCreate.h"

#include "../Skydome/Skydome.h"
#include "../GameObjectsList.h"

IObject* ObjectCreate::CreateObjectSkydome(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Skydome();
	// 初期化
	static_cast<Skydome*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectPlayer(LevelData::ObjectData& objectData)
{
	// 
	IObject* object = new Player();
	//
	static_cast<Player*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectCarCore(LevelData::ObjectData& objectData)
{

	IObject* object = new CarCore();

	static_cast<CarCore*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}
