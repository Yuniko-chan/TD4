#pragma once
#include "../../Utility/Common/OwnerComponent.h"
#include <list>
#include <unordered_map>
#include <map>
#include <string>
#include <utility>

namespace Car { class IParts; }
class VehicleCore;

struct Vector2Int {
	int x, y;
	// 比較演算子
	bool operator<(const Vector2Int& other) const {
		if (x != other.x) return x < other.x;
		return y < other.y;
	}
};

/// <summary>
/// 車両構築システム（コア用）（パーツ管理など）
/// </summary>
class VehicleConstructionSystem : public OwnerComponent<Car::IParts>
{
private:
	static const int kMaxDepth = 5;
public:
	enum Direction {
		kLeft,kRight,kForward,kBackForward,
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
	void Attach(Car::IParts* parts, Direction direct);

	//void AddMapping(const Vector2Int& id);

	Car::IParts* FindPreNumber(std::list<std::pair<int, Car::IParts*>>* directLists, int32_t number);

private:
	// システム用のコア
	VehicleCore* core_ = nullptr;
	// 左
	std::list<std::pair<int, Car::IParts*>> leftD_;
	// 右
	std::list<std::pair<int, Car::IParts*>> rightD_;
	// 前
	std::list<std::pair<int, Car::IParts*>> forwardD_;
	// 後ろ
	std::list<std::pair<int, Car::IParts*>> backForwardD_;

	// 
	std::map<Vector2Int, Car::IParts*> partsMapping_;
	//std::unordered_map<std::pair<int, Vector2>, Car::IParts*>;
};
