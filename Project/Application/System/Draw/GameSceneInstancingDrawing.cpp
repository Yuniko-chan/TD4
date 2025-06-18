#include "GameSceneInstancingDrawing.h"

void GameSceneInstancingDrawing::Initialize()
{

	// モデルデータ挿入

	// モデルデータ<ファイルパス, アニメーションするか>
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/Tire/", "Tire.obj"}, false}); // タイヤ
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/Frame/", "Frame.obj"}, false}); // フレーム
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/Engine/", "Engine.obj"}, false}); // エンジン

	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/Gimmick/Cannon/", "Cannon.obj"}, false}); // 大砲
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/Gimmick/IronBall/", "IronBall.obj"}, false}); // 鉄球
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/Gimmick/IronBall/", "Rope.obj"}, false}); // 鉄球紐
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/Gimmick/Gatling/", "Gatling.gltf"}, false}); // ミニガン

	// モデルデータの数
	modelDataMax_ = modelDatas_.size();

	for (size_t i = 0; i < modelDataMax_; ++i) {
		// インスタンシング描画用モデル保存
		instancingDrawingDatas_.emplace_back();
		// ワールドトランスフォームの保存回数
		instancingDrawingTransformationMatrixNum_.emplace_back();
	}

	// 初期化
	BaseInstancingDrawing::Initialize();

}
