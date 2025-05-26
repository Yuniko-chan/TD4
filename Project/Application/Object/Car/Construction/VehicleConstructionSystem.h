#pragma once
#include "../../Utility/Common/OwnerComponent.h"
#include "../../Utility/Math/Vector2Int.h"
#include <list>
#include <unordered_map>
#include <map>
#include <string>
#include <utility>

namespace Car { class IParts; }
class VehicleCore;

/// <summary>
/// 車両構築システム（コア用）（パーツ管理など）
/// </summary>
class VehicleConstructionSystem : public OwnerComponent<Car::IParts>
{
private:
	struct PartsType
	{
		int16_t tire;
		int16_t armor;
		int16_t engine;
	};
	struct DirectionType {
		int16_t forward;
		int16_t backForward;
		int16_t left;
		int16_t right;
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="core"></param>
	void Initialize(VehicleCore* core);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 方向判断まで
	/// </summary>
	/// <param name="parts"></param>
	void Attach(Car::IParts* parts);

private:
	/// <summary>
	/// 接続
	/// </summary>
	/// <param name="parts"></param>
	void Attach(Car::IParts* parts, const Vector2Int& key);

	/// <summary>
	/// 検索
	/// </summary>
	/// <param name="directLists"></param>
	/// <param name="number"></param>
	/// <returns></returns>
	Car::IParts* FindPreNumber(std::list<std::pair<int, Car::IParts*>>* directLists, int32_t number);
	/// <summary>
	/// パーツの登録処理
	/// </summary>
	/// <param name="parts"></param>
	void RegistParts(const Vector2Int& id, Car::IParts* parts);
	/// <summary>
	/// パーツの解除処理
	/// </summary>
	/// <param name="id"></param>
	/// <param name="parts"></param>
	void UnRegistParts(const Vector2Int& id, Car::IParts* parts);
private:
	void AddCount(std::string name);
	void DeleteCount(std::string name);
private:
	// システム用のコア
	VehicleCore* core_ = nullptr;
	// パーツのリスト
	std::map<Vector2Int, Car::IParts*> partsMapping_;
	// パーツの数
	PartsType counts_;
	// パーツの方角ごとの数
	DirectionType directions_;
public: // アクセッサ
	// 総数
	PartsType GetStatus() { return counts_; }
	DirectionType* GetDirections() { return &directions_; }
};
