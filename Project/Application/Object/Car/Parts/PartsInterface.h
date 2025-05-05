#pragma once
#include "../../../Engine/Object/MeshObject.h"

class VehicleCore;

namespace Car
{
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
		
		void ParentSetting(bool isAccept, const Vector3& offset);
		void TransformParent();
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
		void ColliderUpdate();

	protected:
		// 重さparent
		float weight_ = 1.0f;
		// 親のコア
		VehicleCore* parentCore_ = nullptr;

	};

	///// <summary>
	///// エンジンの基底
	///// </summary>
	//class IEngine : public IParts {



	//};

	///// <summary>
	///// タイヤの基底
	///// </summary>
	//class ITire : public IParts {

	//};

	///// <summary>
	///// フレームの基底
	///// </summary>
	//class IFrame : public IParts {

	//};

}
