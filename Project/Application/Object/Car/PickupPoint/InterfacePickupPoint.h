#pragma once
#include "../../../Engine/Object/MeshObject.h"

class IPickupPoint : public MeshObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="data">メッシュデータ</param>
	void Initialize(LevelData::MeshData* data) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;


private:

};
