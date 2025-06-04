#pragma once
#include "../../../Engine/Object/MeshObject.h"

class IPickupPoint : public MeshObject
{
public:
	enum PickupType {
		kNone,
		kEngine,
		kTire,
		kArmor,
	};

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

	// アクセッサ
	int32_t GetType() { return type_; }

protected:
	// タイプ
	int32_t type_ = kNone;
};
