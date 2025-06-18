#pragma once
#include "../VehicleSystemCommons.h"

class VehicleCore;

class OverheatSystem : public OwnerComponent<VehicleCore>
{
public:

	void Update();

	void SetMappingData(std::map<Vector2Int, Car::IParts*>* data) { mappingData_ = data; }

private:
	/// <summary>
	/// ダメージ処理
	/// </summary>
	void DamageProcess();

	/// <summary>
	/// オーバーヒートの対象を割り当てる
	/// </summary>
	void AssignRandomOverheatPart();

private:
	// パーツのマッピングデータ
	std::map<Vector2Int, Car::IParts*>* mappingData_ = nullptr;
	// 対象のパーツ
	Car::IParts* targetPart_ = nullptr;
	// 前フレームの深度情報（ここが更新されなければダメージ対象を変えない
	int preDepth_ = 0;
	int maxDepth_ = 0;
};