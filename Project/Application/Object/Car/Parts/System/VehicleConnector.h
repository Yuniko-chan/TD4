#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include <list>
#include <array>

// 前方宣言
namespace Car { class IParts; }

class VehicleConnector
{
public:
	~VehicleConnector() = default;
	VehicleConnector() {};
public:
	// 接続方向
	enum Direction {
		forward,	// 前
		backward,	// 後ろ
		left,		// 左
		right,		// 右
		MaxSize,	// 最大値
	};

	/// <summary>
	/// 接続可能かチェックスロット
	/// </summary>
	struct ConnectionSlot {
		bool isConnected = false;	// 接続されているか
		bool isAvailable = true;	// 空か
	};
	
	void Update();

private:
	// 隣接パーツ
	std::list<Car::IParts*> parents_;
	std::list<Car::IParts*> childrens_;
	// 深度値
	int32_t depth_ = 0;
	// 空スロットチェック
	std::array<ConnectionSlot, Direction::MaxSize> connectSlot_;

	// 親ポインタ（座標構築用）
	WorldTransform* coreTransform_ = nullptr;

	bool isDead_ = false;
};
