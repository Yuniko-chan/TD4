#pragma once
#include <string>
#include <variant>
#include <vector>

#include "../3D/Transform/TransformStructure.h"
#include "../Collider/Collider.h"
#include "../Collider/ColliderShape.h"


#include "../../Application/Object/Gimmick/IronBall/PendulumIronBallData.h"
#include "../../Application/Object/Gimmick/Cannon/CannonData.h"
#include "../../Application/Object/Gimmick/Minigun/MinigunData.h"

/// <summary>
/// レベルデータ
/// </summary>
class LevelData
{

public: // サブクラス

	/// <summary>
	/// オブジェクトのデータの種類
	/// </summary>
	enum ObjectDataIndex {
		kObjectDataIndexMesh, // メッシュ
		kObjectDataIndexCamaera, // カメラ
		kObjectDataIndexLight, // ライト
		kObjectDataIndexOfCount // 数数える用
	};

	enum GimmickDataIndex {
		kGimmickDataIndexIronBall,
		kGimmickDataIndexCannon,
		kGimmickDataIndexMinigun,
		kGimmickDataIndexCount
	};

	/// <summary>
	/// メッシュタイプ
	/// </summary>
	struct MeshData
	{

		// 名前
		std::string name;
		// トランスフォーム
		EulerTransform transform;

		// ファイルの名前
		std::string flieName;
		// ディレクトリパス
		std::string directoryPath;
		// クラスの名前
		std::string className;
		// 親の名前
		std::string parentName;

		// コライダー
		ColliderShape collider;

	};

	/// <summary>
	/// カメラタイプ
	/// </summary>
	struct CameraData
	{
		// 名前
		std::string name;
		// トランスフォーム
		EulerTransform transform;
	};


	/// <summary>
	/// ライトタイプ
	/// </summary>
	struct LightData
	{
		// 名前
		std::string name;
		// トランスフォーム
		EulerTransform transform;
	};

	//ギミック

	/// <summary>
	/// 鉄球タイプ
	/// </summary>
	struct IronBallData
	{
		// アンカー
		Vector3 anchor;
		// 紐の長さ
		float length;
		// 円推の頂角の半分
		float angle;
	};



	//ギミックごとの個別データ
	using GimmickSeparateData = std::variant<IronBallData,CannonData,MinigunData>;

	struct GimmickData {
		//メッシュデータ
		MeshData meshData;

		//ギミックデータ
		GimmickSeparateData gimmickSeparateData;
	};

	// 項目をまとめる
	using ObjectData = std::variant<MeshData, CameraData, LightData,GimmickData>;

public: // 変数

	// オブジェクトの情報
	std::vector<ObjectData> objectsData_;

};

