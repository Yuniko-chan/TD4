#include "GameSceneInstancingDrawing.h"

#include "../../Object/Gimmick/Cannon/Cannon.h"
#include "../../Object/Gimmick/IronBall/ConicalPendulumIronBall.h"
#include "../../Object/Gimmick/IronBall/PendulumIronBall.h"
#include "../../Object/Gimmick/Minigun/Minigun.h"

void GameSceneInstancingDrawing::Initialize()
{

	// モデルデータ挿入

	// モデルデータ<ファイルパス, アニメーションするか>
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/Tire/", "Tire.obj"}, false}); // タイヤ
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/Frame/", "Frame.obj"}, false}); // フレーム
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/Engine/", "Engine.obj"}, false}); // エンジン

	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/PickupPoint/Tire/", "tire.obj"}, false}); // タイヤ
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/PickupPoint/Frame", "frame.obj"}, false}); // フレーム
	modelDatas_.push_back(std::pair<ModelPathAndName, bool>{{"Resources/Model/PickupPoint/Engine", "engine.obj"}, false}); // エンジン

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
	// 鉄球
	else if (meshObject->GetClassNameString() == "ConicalPendulumIronBall") {
		ConicalPendulumIronBallVer(meshObject, viewProjectionMatrix);
	}
	// 鉄球
	else if (meshObject->GetClassNameString() == "PendulumIronBall") {
		PendulumIronBallVer(meshObject, viewProjectionMatrix);
	}
	// ミニガン
	else if (meshObject->GetClassNameString() == "Minigun") {
		MinigunVer(meshObject, viewProjectionMatrix);
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

void GameSceneInstancingDrawing::ConicalPendulumIronBallVer(MeshObject* meshObject, const Matrix4x4& viewProjectionMatrix)
{

	ConicalPendulumIronBall* obj = reinterpret_cast<ConicalPendulumIronBall*>(meshObject);

	RopeRegistrationConfirmation(obj->GetStringWorldTransformAddress(), *(obj->GetStringMaterial()->GetMaterialMap()), viewProjectionMatrix);

}

void GameSceneInstancingDrawing::PendulumIronBallVer(MeshObject* meshObject, const Matrix4x4& viewProjectionMatrix)
{

	PendulumIronBall* obj = reinterpret_cast<PendulumIronBall*>(meshObject);

	RopeRegistrationConfirmation(obj->GetStringWorldTransformAddress(), *(obj->GetStringMaterial()->GetMaterialMap()), viewProjectionMatrix);

}

void GameSceneInstancingDrawing::MinigunVer(MeshObject* meshObject, const Matrix4x4& viewProjectionMatrix)
{

	Minigun* obj = reinterpret_cast<Minigun*>(meshObject);

	for (MinigunBullet* bullet : *obj->GetBullets()) {
		RegistrationConfirmation(bullet, viewProjectionMatrix);
	}

}

void GameSceneInstancingDrawing::RopeRegistrationConfirmation(WorldTransform* worldTransform, const MaterialData& materialData, const Matrix4x4& viewProjectionMatrix)
{

	const size_t kRopeNum = 5;

	// トランスフォーム マップ
	instancingDrawingDatas_[kRopeNum].transformMap[instancingDrawingTransformationMatrixNum_[kRopeNum]].WVP = worldTransform->worldMatrix_ * viewProjectionMatrix;
	instancingDrawingDatas_[kRopeNum].transformMap[instancingDrawingTransformationMatrixNum_[kRopeNum]].World = worldTransform->worldMatrix_;
	instancingDrawingDatas_[kRopeNum].transformMap[instancingDrawingTransformationMatrixNum_[kRopeNum]].WorldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldTransform->worldMatrix_));

	// マテリアルデータ マップ
	instancingDrawingDatas_[kRopeNum].materialMap[instancingDrawingTransformationMatrixNum_[kRopeNum]].color = materialData.color;
	instancingDrawingDatas_[kRopeNum].materialMap[instancingDrawingTransformationMatrixNum_[kRopeNum]].enableLighting = materialData.enableLighting;
	instancingDrawingDatas_[kRopeNum].materialMap[instancingDrawingTransformationMatrixNum_[kRopeNum]].environmentCoefficient = materialData.environmentCoefficient;
	instancingDrawingDatas_[kRopeNum].materialMap[instancingDrawingTransformationMatrixNum_[kRopeNum]].shininess = materialData.shininess;
	instancingDrawingDatas_[kRopeNum].materialMap[instancingDrawingTransformationMatrixNum_[kRopeNum]].uvTransform = materialData.uvTransform;

	// 回数をインクリメント
	instancingDrawingTransformationMatrixNum_[kRopeNum]++;

}
