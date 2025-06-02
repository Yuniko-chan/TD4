#include "ObjectCreate.h"

#include "../Skydome/Skydome.h"
#include "../GameObjectsList.h"
#include "../Wall/Wall.h"
#include "../../Course/Course.h"

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

IObject* ObjectCreate::CreateObjectTerrain(LevelData::ObjectData& objectData)
{
	IObject* object = new TerrainObject;

	static_cast<TerrainObject*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectEnginePoint(LevelData::ObjectData& objectData)
{
	IObject* object = new EnginePickupPoint();

	static_cast<EnginePickupPoint*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectTirePoint(LevelData::ObjectData& objectData)
{
	IObject* object = new TirePickupPoint();

	static_cast<TirePickupPoint*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectArmorPoint(LevelData::ObjectData& objectData)
{
	IObject* object = new ArmorPickupPoint();

	static_cast<ArmorPickupPoint*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectWall(LevelData::ObjectData& objectData)
{
	IObject* object = new Wall();

	static_cast<Wall*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectCourse(LevelData::ObjectData& objectData)
{
	IObject* object = new Course();

	static_cast<Course*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectIronBall(LevelData::ObjectData& objectData)
{
	IObject* object = new PendulumIronBall();
	LevelData::IronBallData& ironBallData = std::get<LevelData::IronBallData>(std::get<LevelData::GimmickData>(objectData).gimmickSeparateData);
	PendulumIronBallData data;
	data.anchor = ironBallData.anchor;
	data.angle = ironBallData.angle;
	data.length = ironBallData.length;
	static_cast<PendulumIronBall*>(object)->Initialize(&std::get<LevelData::GimmickData>(objectData).meshData,data);
	return object;
}

IObject* ObjectCreate::CreateObjectCannon(LevelData::ObjectData& objectData)
{
	IObject* object = new Cannon();
	CannonData& data = std::get<CannonData>(std::get<LevelData::GimmickData>(objectData).gimmickSeparateData);

	static_cast<Cannon*>(object)->Initialize(&std::get<LevelData::GimmickData>(objectData).meshData,data);
	return object;
}

IObject* ObjectCreate::CreateObjectMinigun(LevelData::ObjectData& objectData)
{
	IObject* object = new Minigun();
	MinigunData& data = std::get<MinigunData>(std::get<LevelData::GimmickData>(objectData).gimmickSeparateData);

	static_cast<Minigun*>(object)->Initialize(&std::get<LevelData::GimmickData>(objectData).meshData, data);
	return object;
}