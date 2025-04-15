#include "ObjectCreate.h"

#include "../Skydome/Skydome.h"

IObject* ObjectCreate::CreateObjectSkydome(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Skydome();
	// 初期化
	static_cast<Skydome*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}
