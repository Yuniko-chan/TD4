#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"

// 前方宣言
class Player;

/// <summary>
/// 横の布
/// </summary>
class SideCloth :
    public MeshObject
{

public: // 静的メンバ変数

	// 大きさ
	static const Vector2 kClothScale_;
	// 分割数
	static const Vector2 kClothDiv_;
	// ワールド座標からの左固定部分
	static const Vector3 kBaseLeftFixed_;
	// ワールド座標からの右固定部分
	static const Vector3 kBaseRightFixed_;
	// DirectX
	static DirectXCommon* dxCommon_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(BaseCamera& camera) override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw() override;

	/// <summary>
	/// 布リセット
	/// </summary>
	void ClothReset();

private: // メンバ関数

	/// <summary>
	/// 布初期化
	/// </summary>
	void ClothInitialize();

	/// <summary>
	/// 布更新
	/// </summary>
	void ClothUpdate();

public: // アクセッサ

	/// <summary>
	/// プレイヤー設定
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

private: // メンバ変数

	// 布
	std::unique_ptr<ClothGPU> cloth_;

	// プレイヤー
	Player* player_;

	// 更新秒数
	float updateSeconds_;

};

