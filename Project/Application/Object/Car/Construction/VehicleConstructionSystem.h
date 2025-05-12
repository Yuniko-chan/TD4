#pragma once
#include "../../Utility/Common/OwnerComponent.h"
#include <list>
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

};
