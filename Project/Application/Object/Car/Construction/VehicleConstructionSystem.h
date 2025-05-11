#pragma once

#include <list>
#include <string>
#include <utility>

namespace Car { class IParts; }
class VehicleCore;

/// <summary>
/// 車両構築システム（コア用）（パーツ管理など）
/// </summary>
class VehicleConstructionSystem
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

private:
	// システム用のコア
	VehicleCore* core_ = nullptr;

	// ４方向ディレクト
	//std::map<std::string, std::pair<int, Car::IParts*>> fourDirection_;

	// 左
	//std::array<std::pair<int, Car::IParts*>, kMaxDepth> leftD_;
	std::list<std::pair<int, Car::IParts*>> leftD_;
	// 右
	//std::array<std::pair<int, Car::IParts*>, kMaxDepth> rightD_;
	std::list<std::pair<int, Car::IParts*>> rightD_;
	// 前
	//std::array<std::pair<int, Car::IParts*>, kMaxDepth> forwardD_;
	std::list<std::pair<int, Car::IParts*>> forwardD_;
	// 後ろ
	//std::array<std::pair<int, Car::IParts*>, kMaxDepth> backforwardD_;
	std::list<std::pair<int, Car::IParts*>> backForwardD_;

};
