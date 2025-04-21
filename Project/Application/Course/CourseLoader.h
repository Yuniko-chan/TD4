#pragma once
#include "../../Engine/Math/Vector/Vector2.h"
#include "../../Engine/Math/Vector/Vector3.h"
#include "../../Engine/Math/Vector/Vector4.h"

#include <string>
#include <fstream>

static const uint32_t kVerticesMax = 8192;
static const uint32_t kInportFileSize = 65536;
//コースファイル形式の頂点データ
struct CourseFileVertex
{
	Vector4 positon_;
	Vector2 uv_;
	Vector3 normal_;
	uint32_t coruseAttribute_;
};

//コース用読み取りデータ
struct CourseInportData
{
	uint32_t verticesNum_;
	uint32_t textureNameSize_;
	CourseFileVertex vertices[kVerticesMax];
	std::string textureFileName_;
};

class CourseLoader
{
public:
	/// <summary>
	/// courseファイルを読む
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイルの名前</param>
	/// <returns></returns>
	static void LoadCourseFile(const std::string& directoryPath, const std::string& filename);

private:
	/// <summary>
	/// インポート用データを一時リソースに格納する
	/// </summary>
	static void StragedInportData(FILE* fp, CourseInportData* courseInportData);
};
