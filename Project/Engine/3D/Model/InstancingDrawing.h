#pragma once
#include <vector>
#include "../Transform/WorldTransform.h"
#include "InstancingDrawingData.h"
#include <list>

/// <summary>
/// インスタンシング描画
/// </summary>
class InstancingDrawing
{

private: //定数

	/// <summary>
	/// モデルデータ最大数
	/// </summary>
	static const size_t kModelDataMax_ = 2;

	/// <summary>
	/// それぞれのトランスフォームデータ最大数
	/// </summary>
	static const size_t kTransformationMatrixMax_ = 64;

	/// <summary>
	/// モデルのパスと名前
	/// </summary>
	using ModelPathAndName = std::pair<std::string, std::string>;

	/// <summary>
	/// モデルデータ<ファイルパス,アニメーションするか>
	/// </summary>
	const std::array<std::pair<ModelPathAndName, bool>, kModelDataMax_> kModelDatas_ = {
	
		std::pair<ModelPathAndName, bool>{{"aaa", "aaa"}, false},
		std::pair<ModelPathAndName, bool>{{"aaa", "aaa"}, false},
	
	};

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// クリア
	/// </summary>
	void Clear();

	/// <summary>
	/// 登録確認
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
	/// <returns>成功したか</returns>
	bool RegistrationConfirmation(Model* model,	WorldTransform* worldTransform, const Matrix4x4& viewProjectionMatrix);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

private:


	// インスタンシング描画用モデル保存
	std::array<InstancingDrawingData, kModelDataMax_> instancingDrawingDatas_;

	// ワールドトランスフォームの保存回数
	std::array<size_t, kModelDataMax_> instancingDrawingTransformationMatrixNum_;

};

