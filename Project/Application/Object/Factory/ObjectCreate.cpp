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

IObject* ObjectCreate::CreateObjectVehicleCore(LevelData::ObjectData& objectData)
{

	IObject* object = new VehicleCore();

	static_cast<VehicleCore*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectEngineParts(LevelData::ObjectData& objectData)
{
	IObject* object = new EngineParts();

	static_cast<EngineParts*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectTireParts(LevelData::ObjectData& objectData)
{
	IObject* object = new TireParts();

	static_cast<TireParts*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectArmorFrameParts(LevelData::ObjectData& objectData)
{
	IObject* object = new ArmorFrameParts();

	static_cast<ArmorFrameParts*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectIParts(LevelData::ObjectData& objectData)
{
	IObject* object = new Car::IParts();

	static_cast<Car::IParts*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}
