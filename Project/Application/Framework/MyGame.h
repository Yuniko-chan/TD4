#pragma once

// フレームワーク
#include "../../Engine/Framework/MyFramework.h"

#include "../Scene/SceneManager/SceneManager.h"

/// <summary>
/// 自作エンジン名前空間
/// </summary>
namespace NomeEngine {

	/// <summary>
	/// ゲーム大枠
	/// </summary>
	class MyGame :
		public MyFramework
	{

	public: // メンバ関数

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Finalize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() override;

		/// <summary>
		/// 描画処理
		/// </summary>
		void Draw() override;

	private: // メンバ変数

		// シーンマネージャー
		std::unique_ptr<SceneManager> sceneManager = nullptr;

	};

};

