#pragma once
#include <algorithm>

using GridSize = std::pair<int, int>;

class CameraHelper
{
public:
	/// <summary>
	/// グリッドサイズに適したオフセット作成
	/// </summary>
	/// <param name="grid">マッピングサイズ</param>
	/// <param name="threshold">上限閾値</param>
	/// <param name="offsetRatio">オフセットの値</param>
	/// <returns></returns>
	float CreateGridSizeOffset(const GridSize& grid, const int& threshold, float offsetRatio = 1.0f);

private:
	/// <summary>
	/// 横幅計算
	/// </summary>
	/// <param name="x">グリッドのサイズ</param>
	/// <param name="threshold">閾値</param>
	/// <param name="offsetRatio">倍率</param>
	/// <returns></returns>
	float CreateGridWidthOffset(const int& grid, const int& threshold, float offsetRatio = 1.0f);
	/// <summary>
	/// 奥行き計算
	/// </summary>
	/// <param name="y">グリッドのサイズ</param>
	/// <param name="threshold">閾値</param>
	/// <param name="offsetRatio">倍率</param>
	/// <returns></returns>
	float CreateGridDepthOffset(const int& grid, const int& threshold, float offsetRatio = 1.0f);

};
