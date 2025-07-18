#pragma once
#include "Model.h"
#include "../../Animation/Animation.h"

/// <summary>
/// インスタンシング描画アニメーション
/// </summary>
class BaseInstancingDrawingAnimation
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル/param>
	virtual void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stateMotionNo">ステートのモーション番号</param>
	void Update(uint32_t stateMotionNo);

public: // アクセッサ

	/// <summary>
	/// ローカル行列取得
	/// </summary>
	/// <returns></returns>
	LocalMatrixManager* GetLocalMatrixManager() { return localMatrixManager_.get(); }

protected: // メンバ変数

	// 現在のモーション番号
	uint32_t currentMotionNo_;

	// 前のモーション番号
	uint32_t prevMotionNo_;

	//ノードアニメーション
	Animation animation_;

	// ローカル行列マネージャ
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;

};

