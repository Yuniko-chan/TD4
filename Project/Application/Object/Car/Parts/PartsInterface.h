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

		///// <summary>
		///// 描画
		///// </summary>
		///// <param name="camera">カメラ</param>
		//void Draw(BaseCamera& camera) override;

		void SetParent(VehicleCore* parent) { parentCore_ = parent; }

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
