#pragma once
#include "../../Engine/Math/Vector/Vector2.h"
#include "../../Engine/Math/Vector/Vector3.h"
#include "../../Engine/Math/Vector/Vector4.h"
#include "../../Engine/3D/Model/Model.h"
#include "../../Engine/3D/Model/ModelLoader.h"

#include "CoursePolygon.h"

#include <string>
#include <fstream>

static const uint32_t kVerticesMax = 8192;
static const uint32_t kInportFileSize = 65536*2;
static const size_t kCourseAttributeOffset = 6;
static const size_t kHeaderOffset = sizeof(uint32_t) * 2 + sizeof(bool) * kCourseAttributeOffset;
//コースファイル形式の頂点データ
struct CourseFileVertex
{
	Vector4 position_;
	Vector2 uv_;
	Vector2 attributeUv_;
	Vector3 normal_;
	//uint32_t courseAttribute_;
};

//コース用読み取りデータ
struct CourseImportData
{
	uint32_t verticesNum_;
	uint32_t textureNameSize_;
	bool courseAttribute_[kCourseAttributeOffset] = {false};
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
	/// 
	/// <returns></returns>
	static Model* LoadCourseFile(const std::string& directoryPath, const std::string& filename,std::vector<CoursePolygon>& course);

private:
	/// <summary>
	/// インポート用データを一時リソースに格納する
	/// </summary>
	static void StragedImportData(FILE* fp, CourseImportData* courseInportData);
};
