#pragma once
#include "../../../Utility/Common/OwnerComponent.h"
//#include "../../Player.h"
#include <list>
#include <algorithm>

namespace Car { class IParts; }
class VehiclePartsManager;
class PickupPointManager;
class Player;
class MeshObject;


class PartJudgeSystem : public OwnerComponent<Player>
{
public:
	// 状態
	enum PartCondition
	{
		Dropped,
		Docked,
		Core,
		PickupPoint,
	};

	struct ConditionData
	{
		MeshObject* object;
		PartCondition type;
		float distance;
		bool isFront;
	};


public:
	/// <summary>
	/// パーツを持つ処理
	/// </summary>
	/// <param name="partManager"></param>
	/// <param name="pointManager"></param>
	/// <returns></returns>
	Car::IParts* GetCatchPart(VehiclePartsManager* partManager, PickupPointManager* pointManager);

private:
	/// <summary>
	/// 一番近いのをソートして取得
	/// </summary>
	/// <returns></returns>
	ConditionData NearSort();
	/// <summary>
	/// 拾うパーツを決定する
	/// </summary>
	/// <param name="partManager"></param>
	/// <param name="pointManager"></param>
	/// <param name="data"></param>
	/// <returns></returns>
	Car::IParts* GetCatchPart(PickupPointManager* pointManager, ConditionData data);
private:
	// リスト
	std::list<ConditionData> objects_;
};
