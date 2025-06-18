#include "GameSceneInstancingDrawing.h"

#include "../../Object/Gimmick/Cannon/Cannon.h"

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

	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/defult", "ball.obj"}, false}); // ball


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

bool GameSceneInstancingDrawing::RegistrationConfirmation(MeshObject* meshObject, const Matrix4x4& viewProjectionMatrix)
{

	// クラス確認
	// 大砲
	if (meshObject->GetClassNameString() == "Cannon") {
		CannonVer(meshObject, viewProjectionMatrix);
	}

	return BaseInstancingDrawing::RegistrationConfirmation(meshObject, viewProjectionMatrix);
}

void GameSceneInstancingDrawing::CannonVer(MeshObject* meshObject, const Matrix4x4& viewProjectionMatrix)
{

	Cannon* obj = reinterpret_cast<Cannon*>(meshObject);

	CannonBall* cannonBall = obj->GetCannonBall();

	CannonExplosion* cannonExplosion = obj->GetCannonBall()->GetCannonExplosion();

	// 弾
	if (cannonBall->GetIsWorking()) {
		RegistrationConfirmation(obj->GetCannonBall(), viewProjectionMatrix);
	}

	// 爆発
	if (cannonExplosion->GetIsExploding()) {
		RegistrationConfirmation(obj->GetCannonBall()->GetCannonExplosion(), viewProjectionMatrix);
	}

}
