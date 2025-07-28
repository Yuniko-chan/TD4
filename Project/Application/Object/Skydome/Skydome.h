#pragma once
#include "../../../Engine/Object/MeshObject.h"

// 前方宣言
class Player;

/// <summary>
/// スカイドーム
/// </summary>
class Skydome : 
	public MeshObject 
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="data">データ</param>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 親設定
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }

private: // メンバ変数

	// 回転速度
	const float kRotateSpeed_ = 0.001f;

};