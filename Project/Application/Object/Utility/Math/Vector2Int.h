#pragma once
#include <cmath>

struct Vector2Int {
	int x, y;
	// 比較演算子
	bool operator<(const Vector2Int& other) const {
		if (x != other.x) return x < other.x;
		return y < other.y;
	}

	/// <summary>
	/// 長さ取得
	/// </summary>
	/// <returns></returns>
	int GetLength() {
		return int(std::abs(x) + std::abs(y));
	}

};
