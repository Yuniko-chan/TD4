#pragma once
#include "../../Engine/Object/MeshObject.h"
#include "Course.h"
#include "../../Engine/base/DescriptorHandles.h"

/// <summary>
/// コース衝突システム
/// </summary>
class CourseCollisionSystem
{

private: // メンバ定数

	// 登録できるオブジェクトの数
	static const int kObjectsThatCanBeRegisteredMax_ = 8;
	
	// ブロードフェーズで使用する、距離判定
	static const float kDistanceJudgment_;
	
	//	ポリゴンエリアの分割数
	static const int kPolygonAreasDiv_ = 4;
	// ポリゴンエリアの原点
	static const Vector3 kPolygonAreasOrigin_;
	// ポリゴンエリアの長さ
	static const Vector3 kPolygonAreasLength_;
	
	/// <summary>
	///	オブジェクトデータ(現在OBB)
	/// </summary>
	struct ObjectData
	{
		// 中心
		Vector3 center;
		// 座標軸
		Vector3 otientatuons[3];
		// 座標軸方向の長さの半分
		Vector3 size;
	};

	/// <summary>
	///	出力データ
	/// </summary>
	struct OutputData
	{

	};

	/// <summary>
	/// バッファ(登録できるオブジェクトの数分)
	/// </summary>
	struct Buffers
	{
		// オブジェクト(SRV)
		Microsoft::WRL::ComPtr<ID3D12Resource> objectBuff_;
		CourseCollisionSystem::ObjectData* objectMap_ = nullptr;
		DescriptorHandles objectDescriptorHandles;

		// 衝突するかもしれないポリゴンデータ（SRV）
		Microsoft::WRL::ComPtr<ID3D12Resource> polygonDataBuff_;
		CoursePolygon* polygonDataMap_ = nullptr;
		DescriptorHandles polygonDataDescriptorHandles;

		// 押し出しと走行場所のデータ（UAV）
		Microsoft::WRL::ComPtr<ID3D12Resource> outputDataBuff_;
		OutputData* outputDataMap_ = nullptr;
		DescriptorHandles outputDescriptorHandles;

	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 実行
	/// </summary>
	void Execute();

	/// <summary>
	/// コースとぶつかるオブジェクト登録
	/// </summary>
	/// <param name="object">オブジェクトのポインタ</param>
	void ObjectRegistration(MeshObject* object);

	/// <summary>
	/// コース設定
	/// </summary>
	/// <param name="course">コース</param>
	void SetCourse(Course* course);

private: // メンバ変数
	
	/// <summary>
	/// 距離判定
	/// </summary>
	/// <param name="object">オブジェクト</param>
	void DistanceJudgment(MeshObject* object);

private: // メンバ変数

	// コース
	Course* course_;

	// コースとぶつかるオブジェクト
	std::list<MeshObject*> collidingObjects_;

	// バッファ群
	Buffers buffers_;

	// エリア
	std::vector<CoursePolygon> polygonAreas[kPolygonAreasDiv_][kPolygonAreasDiv_][kPolygonAreasDiv_];

};

