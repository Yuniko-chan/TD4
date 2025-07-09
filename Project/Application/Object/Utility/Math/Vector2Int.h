#pragma once
#include <cmath>

struct Vector2Int {
	int x, y;
	// 比較演算子
	bool operator<(const Vector2Int& other) const {
		if (x != other.x) return x < other.x;
		return y < other.y;
	}

	bool operator==(const Vector2Int& other) const {
		if (x == other.x && y == other.y) {
			return true;
		}
		return false;
	}

	bool operator!=(const Vector2Int& other) const {
		if (x != other.x || y != other.y) {
			return true;
		}
		return false;
	}

	Vector2Int operator+(const Vector2Int& other) const {
		Vector2Int result = *this;
		result.x += other.x;
		result.y += other.y;
		return result;
	}

	/// <summary>
	/// 長さ取得
	/// </summary>
	/// <returns></returns>
	int GetLength() const{
		return int(std::abs(x) + std::abs(y));
	}

};
