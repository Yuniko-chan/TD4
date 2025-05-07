#pragma once
#include "../../../Engine/Object/MeshObject.h"

class VehicleCore;
class VehiclePartsManager;

namespace Car
{
	//---パーツごとのシリアル---//
	// エンジン
	static int32_t sSerialEngine = 0;
	// アーマー
	static int32_t sSerialArmor = 0;
	// タイヤ
	static int32_t sSerialTire = 0;
	// コア
	static int32_t sSerialCore = 0;

	/// <summary>
	/// 基底パーツ
	/// </summary>
	class IParts : public MeshObject {
	public:
        /// <summary>
        /// 初期化
        /// </summary>
        /// <param name="data">メッシュデータ</param>
        void Initialize(LevelData::MeshData* data) override;
		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;

		// 親子関係
		void SetParent(VehicleCore* parent) { parentCore_ = parent; }
		// 親があるか
		bool IsParent() { return worldTransform_.parent_ ? true : false; }
		// ワールドトランスフォームの親設定
		void TransformParent();
		// 親の設定
		void SettingParent(VehiclePartsManager* partsManager);
		// ワールドトランスフォームの親解除
		void ReleaseParent();

		/// <summary>
		/// ImGui描画（パーツマネージャにまとめる用のもの
		/// </summary>
		virtual void ImGuiDrawParts() {};

	protected:
		/// <summary>
		/// ImGui
		/// </summary>
		void ImGuiTransform(const float& value);
		/// <summary>
		/// 親を所持するパーツ用
		/// </summary>
		void ImGuiDrawChildParts();
		/// <summary>
		/// コライダーの更新
		/// </summary>
		void ColliderUpdate();

	protected:
		/// <summary>
		/// 子専用の処理
		/// </summary>
		void ChildUpdate();

	protected:
		// 重さ
		float weight_ = 1.0f;
		// 親のコア
		VehicleCore* parentCore_ = nullptr;

	};
}
