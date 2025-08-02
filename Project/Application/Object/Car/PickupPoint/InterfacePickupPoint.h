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
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(BaseCamera& camera) override;
	// アクセッサ
	int32_t GetType() { return type_; }

protected:
	// タイプ
	int32_t type_ = kNone;
};
