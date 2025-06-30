#pragma once
#include <vector>
#include "../Transform/WorldTransform.h"
#include "InstancingDrawingData.h"
#include <list>
#include "../../Object/MeshObject.h"
#include "BaseInstancingDrawingAnimation.h"

/// <summary>
/// インスタンシング描画
/// </summary>
class BaseInstancingDrawing
{

protected: //定数

	/// <summary>
	/// それぞれのトランスフォームデータ最大数
	/// </summary>
	static const size_t kTransformationMatrixMax_ = 64;

	/// <summary>
	/// モデルのパスと名前
	/// </summary>
	using ModelPathAndName = std::pair<std::string, std::string>;

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// クリア
	/// </summary>
	void Clear();

	/// <summary>
	/// 登録確認
	/// </summary>
	/// <param name="meshObject">メッシュオブジェクト</param>
	/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
	/// <returns>成功したか</returns>
	virtual bool RegistrationConfirmation(
		MeshObject* meshObject,
		const Matrix4x4& viewProjectionMatrix);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(BaseCamera& camera);

	/// <summary>
	/// ローカル行列マネージャー設定
	/// </summary>
	/// <param name="fileName">ファイル名前</param>
	/// <param name="localMatrixManager">ローカル行列マネージャー</param>
	void SetLocalMatrixManager(const std::string& fileName, LocalMatrixManager* localMatrixManager);

protected: // 変数

	// モデルデータ最大数
	size_t modelDataMax_;

	// モデルデータ<ファイルパス, アニメーションするか>
	std::vector<std::pair<ModelPathAndName, bool>> modelDatas_;

	// インスタンシング描画用モデル保存
	std::vector<InstancingDrawingData> instancingDrawingDatas_;

	// ワールドトランスフォームの保存回数
	std::vector<size_t> instancingDrawingTransformationMatrixNum_;

	// アニメーション
	std::vector<std::unique_ptr<BaseInstancingDrawingAnimation>> animations_;

};

