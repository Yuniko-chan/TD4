#include "CourseLoader.h"
#include <stdio.h>
Model* CourseLoader::LoadCourseFile(const std::string& directoryPath, const std::string& filename, std::vector<CoursePolygon>& course) {

	//モデル生成
	Model* newModel = nullptr;

	//ファイルオープン
	std::string filepath = directoryPath + "/" + filename;
	FILE* fp;
	
	if (fopen_s(&fp, filepath.c_str(), "rb")==0) {
		//一時リソース生成
		CourseImportData* data = new CourseImportData();

		//一時リソース格納
		StragedImportData(fp, data);

		fclose(fp);

		//モデルデータ作製
		Model::ModelData modelData;
		modelData.material.textureFilePaths.push_back(directoryPath + "/" + data->textureFileName_);
		
		CoursePolygon cData;
		//uint32_t courseAttribute[3] = {0};

		//データ格納
		for (uint32_t i = 0; i < data->verticesNum_; i++) {
			VertexData vData;
			vData.position = data->vertices[i].position_;
			vData.texcoord = data->vertices[i].uv_;
			vData.normal = data->vertices[i].normal_;

			VertexInfluence vertexInfluence;
			for (uint32_t j = 1; j < kNumMaxInfluence; ++j) {
				vertexInfluence.weights[j] = 0.0f;
				vertexInfluence.indecis[j] = 1000;
			}
			vertexInfluence.indecis[0] = 0; // 親ノード分＋1
		
			modelData.vertexInfluences.push_back(vertexInfluence);
			modelData.indices.push_back(i);
			modelData.vertices.push_back(vData);
			
			//coursepolygone
			if (i % 3 == 0) {
				cData.position0 = { data->vertices[i].position_.x, data->vertices[i].position_.y, data->vertices[i].position_.z };
			}
			else if (i % 3 == 1) {
				cData.position1 = { data->vertices[i].position_.x, data->vertices[i].position_.y, data->vertices[i].position_.z };
			}
			else {
				cData.position2 = { data->vertices[i].position_.x, data->vertices[i].position_.y, data->vertices[i].position_.z };
			}

			cData.normal += data->vertices[i].normal_;
			cData.texcoord += data->vertices[i].attributeUv_;
			//courseAttribute[i % 3] = data->vertices[i].courseAttribute_;
			if (i%3 == 2) {
				cData.normal = Vector3::Normalize(cData.normal);
				//cData.coursePolygonType = std::min(std::min(courseAttribute[0], courseAttribute[1]), courseAttribute[2]);
				//cData.texcoord = data->vertices[i].uv_;
				cData.texcoord = cData.texcoord * (1.0f / 3.0f);
				// texcoordのｙがマイナスになっているため
				cData.texcoord.y *= -1.0f;
				course.push_back(cData);
				cData.normal = {0,0,0};
				cData.texcoord = { 0,0 };
			}
		}

		//モデル生成
		newModel = Model::CreateFromModelData(modelData,filename);


		//一時リソース削除
		delete data;
	}

	return newModel;
}


void CourseLoader::StragedImportData(FILE* fp, CourseImportData* courseImportData) {
	static char buff[kInportFileSize];
	fread(buff, sizeof(char) * kInportFileSize, 1, fp);
	//ヘッダ
	memcpy(&(courseImportData->verticesNum_), buff, sizeof(uint32_t));
	memcpy(&(courseImportData->textureNameSize_), buff + sizeof(uint32_t), sizeof(uint32_t));
	memcpy(&(courseImportData->courseAttribute_), buff + (sizeof(uint32_t)*2), sizeof(bool)*kCourseAttributeOffset);

	//頂点
	for (uint32_t i = 0; i < courseImportData->verticesNum_;i+=3) {
		//開始オフセット
		size_t offset = kHeaderOffset + sizeof(CourseFileVertex) * size_t(i);
		memcpy(&(courseImportData->vertices[i]), buff + offset, sizeof(CourseFileVertex));
		offset +=sizeof(CourseFileVertex) * 2;
		memcpy(&(courseImportData->vertices[i+1]), buff + offset, sizeof(CourseFileVertex));
		offset -=sizeof(CourseFileVertex);
		memcpy(&(courseImportData->vertices[i+2]), buff + offset, sizeof(CourseFileVertex));
	}
	//テクスチャファイルネーム
	size_t nameOffset = kHeaderOffset + sizeof(CourseFileVertex) * size_t(courseImportData->verticesNum_);
	static char name[256];
	memcpy(&(name), buff + nameOffset, size_t(courseImportData->textureNameSize_));
	courseImportData->textureFileName_ = name;
}