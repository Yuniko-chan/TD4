#pragma once
#include "../../../Engine/Object/MeshObject.h"

class InteractionSpot : public MeshObject
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

public:	// アクセッサ
	void SetIsDraw(bool isDraw) { isDraw_ = isDraw; }

private:
	bool isDraw_ = false;

};
