#pragma once
#include "../../Engine/Object/MeshObject.h"
#include "Course.h"
#include "../../Engine/base/DescriptorHandles.h"
#include "CourseCollisionPipeline.h"
#include "../../Engine/Object/BaseObjectManager.h"
#include <variant>

// 前方宣言
class CourseDemoObject;
class Player;
class VehicleCore;
class TireParts;
class ArmorFrameParts;
class EngineParts;

/// <summary>
/// コース衝突システム
/// </summary>
class CourseCollisionSystem
{

private: // メンバ定数

	// 衝突するオブジェクト
	using CollisionCarObject = std::variant<VehicleCore*, TireParts*, ArmorFrameParts*, EngineParts*>;
	using CollisionObject = std::variant<Player*, CourseDemoObject*, CollisionCarObject>;

	// 登録できるオブジェクトの数
	static const uint32_t kObjectsThatCanBeRegisteredMax_ = 64;
	
	// 衝突するポリゴンの上限
	static const uint32_t kCollisionPolygonMax_ = 4096;
	
	/// <summary>
	///	オブジェクトデータ(現在OBB)
	/// </summary>
	struct ObjectData
	{		
		// 番号Max
		uint32_t indexMax;
		// 中心
		Vector3 center;
		// 座標軸方向の長さの半分
		Vector3 size;
		// 座標軸
		Vector3 planeYZ;
		Vector3 planeXZ;
		Vector3 planeXY;
	};

	/// <summary>
	///	出力データ
	/// </summary>
	struct OutputData
	{
		// 押し出し
		Vector3 extrusion;
		// 走行場所
		uint32_t drivingLocation;
		// 衝突したか
		uint32_t collided;

	};

	/// <summary>
	/// バッファ(登録できるオブジェクトの数分)
	/// </summary>
	struct Buffers
	{
		// オブジェクト(CBV)
		Microsoft::WRL::ComPtr<ID3D12Resource> objectBuff_;
		CourseCollisionSystem::ObjectData* objectMap_ = nullptr;

		// 衝突するかもしれないポリゴンデータ（SRV）
		Microsoft::WRL::ComPtr<ID3D12Resource> polygonDataBuff_;
		CoursePolygon* polygonDataMap_ = nullptr;
		DescriptorHandles polygonDataDescriptorHandles;

		// 押し出しと走行場所のデータ（UAV）
		Microsoft::WRL::ComPtr<ID3D12Resource> outputDataBuff_;
		OutputData* outputDataMap_ = nullptr;
		DescriptorHandles outputDescriptorHandles;

	};

	// 衝突するオブジェクトキーワード
	static const uint32_t kCollidingObjectKeywordsMax_ = 6;
	static const std::array<std::string, kCollidingObjectKeywordsMax_> kCollidingObjectKeywords_;

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
	void ObjectRegistration(CollisionObject object);

	/// <summary>
	/// コースとぶつかるオブジェクト登録
	/// </summary>
	/// <param name="objectManager">オブジェクトマネージャー</param>
	void ObjectRegistration(BaseObjectManager* objectManager);

	/// <summary>
	/// コース設定
	/// </summary>
	/// <param name="course">コース</param>
	void SetCourse(Course* course);

	/// <summary>
	/// カスタムエリア設定
	/// </summary>
	/// <param name="customArea">カスタムエリア</param>
	void SetCustomArea(CustomArea* customArea);

	/// <summary>
	/// ギミック設定
	/// </summary>
	/// <param name="obb">ギミックOBB</param>
	void SetGimmick(OBB* obb);

	/// <summary>
	/// コースクリア
	/// </summary>
	void ClearCorse();

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

private: // メンバ変数
	
	/// <summary>
	/// バッファ初期化
	/// </summary>
	void BuffersInitialize();

	/// <summary>
	/// 距離判定
	/// </summary>
	/// <param name="object">オブジェクト</param>
	void DistanceJudgment(CollisionObject object);

	/// <summary>
	/// 押し出し処理実行
	/// </summary>
	void ExtrusionExecuteCS();

	/// <summary>
	/// コマンドキック
	/// </summary>
	void CommadKick();

	/// <summary>
	/// 単独押し出し計算
	/// </summary>
	/// <param name="object">オブジェクト</param>
	void AloneExtrusionCalculation(CollisionObject object);

	/// <summary>
	/// カート押し出し計算
	/// </summary>
	void CartExtrusionCalculation();

	/// <summary>
	/// オブジェクト登録プレイヤー版
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void ObjectRegistrationPlayer(Player* player);

private: // メンバ変数

	// コースとぶつかるオブジェクト
	std::list<CollisionObject> collidingObjects_;

	// バッファ群
	Buffers buffers_[kObjectsThatCanBeRegisteredMax_];

	// エリア
	std::array<CoursePolygon, kCollisionPolygonMax_> polygons_;

	// ポリゴン登録番号
	uint32_t polygonRegistrationNumber_;

	// 現在確認しているオブジェクトが何番目か
	uint32_t collisionCheakNum_;

	// パイプライン
	std::unique_ptr<CourseCollisionPipeline> courseCollisionPipeline_;

	// DxCommon
	DirectXCommon* dxCommon_;

	// カートに属しているパーツ&&衝突しているパーツの番号保存
	std::list<uint32_t> belongsToCartPartsNumbers_;

	// コア
	VehicleCore* vehicleCore_;

	// 衝突用テクスチャ
	uint32_t roadAttributeTextureHandle_;

};
