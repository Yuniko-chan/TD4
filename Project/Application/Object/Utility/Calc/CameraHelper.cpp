#include "CameraHelper.h"

float CameraHelper::CreateGridSizeOffset(const GridSize& grid, const int& threshold, float offsetRatio)
{
	// オフセット生成（XとYで）
	// 特異処理
	if (grid.first > threshold && grid.second > threshold) {
		int rec = grid.first - grid.second;
		if (rec <= -5) {
			return CreateGridDepthOffset(grid.second, threshold, offsetRatio);
		}
		return CreateGridWidthOffset(grid.first, threshold, offsetRatio);
	}
	// 横幅
	else if (grid.first > threshold) {
		return CreateGridWidthOffset(grid.first, threshold, offsetRatio);
	}
	// 奥行き
	else if (grid.second > threshold) {
		return CreateGridDepthOffset(grid.second, threshold, offsetRatio);
	}

	return 0.0f;
}

float CameraHelper::CreateGridWidthOffset(const int& grid, const int& threshold, float offsetRatio)
{
	float result = 0.0f;
	const float kWidthRatio = 2.0f;
	result = ((float)grid - (float)threshold) * (offsetRatio * kWidthRatio);
	return result;
}

float CameraHelper::CreateGridDepthOffset(const int& grid, const int& threshold, float offsetRatio)
{
	float result = 0.0f;
	result = ((float)grid - (float)threshold) * offsetRatio;
	return result;
}
