#include "CourseLoader.h"
#include <stdio.h>
void CourseLoader::LoadCourseFile(const std::string& directoryPath, const std::string& filename) {
	//一時リソース生成
	CourseInportData* data = new CourseInportData();

	//ファイルオープン

	std::string filepath = directoryPath + "/" + filename;
	FILE* fp;
	
	if (fopen_s(&fp, filepath.c_str(), "rb")==0) {
		//一時リソース格納
		StragedInportData(fp, data);

		fclose(fp);

	}
	//一時リソース削除
	delete data;
}


void CourseLoader::StragedInportData(FILE* fp, CourseInportData* courseInportData) {
	static char buff[kInportFileSize];
	fread(buff, sizeof(char) * kInportFileSize, 1, fp);
	//ヘッダ
	memcpy(&(courseInportData->verticesNum_), buff, sizeof(uint32_t));
	memcpy(&(courseInportData->textureNameSize_), buff + sizeof(uint32_t), sizeof(uint32_t));
	//頂点
	for (uint32_t i = 0; i < courseInportData->verticesNum_;i++) {
		//開始オフセット
		size_t offset = sizeof(uint32_t)*2 + sizeof(CourseFileVertex) * size_t(i);
		courseInportData->vertices[i];
		memcpy(&(courseInportData->vertices[i]), buff + offset, sizeof(CourseFileVertex));
	}
	//テクスチャファイルネーム
	size_t nameOffset = sizeof(uint32_t) * 2 + sizeof(CourseFileVertex) * size_t(courseInportData->verticesNum_);
	static char name[256];
	memcpy(&(name), buff + nameOffset, sizeof(buff)-nameOffset);
	courseInportData->textureFileName_ = name;
}