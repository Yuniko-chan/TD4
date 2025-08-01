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
	void SetColor(const Vector3& color) { color_ = color; }
	void SetAlpha(float alpha) { alpha_ = alpha; }
private:
	bool isDraw_ = false;
	Vector3 color_ = Vector3(1, 1, 1);
	float alpha_ = 1.0f;
};
