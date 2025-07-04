#pragma once

#include "../../../Engine/Scene/BaseScene/BaseScene.h"
#include "../../Object/Skydome/Skydome.h"
#include "../../../Engine/Physics/Cloth/Cloth.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"
#include "../../ClothDemo/ClothDemo.h"
#include "../../Course/Course.h"
#include "../../Course/CourseCollisionSystem.h"
#include "../../Course/Demo/CourseDemoObject.h"
#include "../../Object/Gimmick/IronBall/ConicalPendulumIronBall.h"
#include "../../Object/Gimmick/IronBall/PendulumIronBall.h"
#include "../../Object/Gimmick/Cannon/Cannon.h"
#include "../../Object/Gimmick/Minigun/Minigun.h"
#include "../../Object/Gimmick/Obstacle/Obstacle.h"

#include "../../Object/Player/Player.h"

#include "../../../Engine/GPUParticle/GPUParticle.h"
#include "../../ParticleManager/ParticleManager.h"
#include "../../UI/UIManager.h"

/// <summary>
/// デバッグシーン
/// </summary>
class DebugScene :
	public BaseScene
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DebugScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();


public: //メンバ関数

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
	void DebugCameraUpdate();

private: // メンバ変数

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	// Course
	std::unique_ptr<Course> course_;
	std::unique_ptr<Model> courseModel_;

	// CourseCollisionSystem 
	std::unique_ptr<CourseCollisionSystem> courseCollisionSystem_;

	// コースとぶつけるオブジェクト
	std::unique_ptr<CourseDemoObject> courseDemoObject_;
	std::unique_ptr<Model> courseDemoModel_;

	// 障害物
	std::unique_ptr<Cannon> objG_;
	std::unique_ptr<Model> objModel_;
	// クロスシミュレーションデモ
	//std::unique_ptr<ClothDemo> clothDemo_;

	std::unique_ptr<ParticleManager> ParticleManager_;
	std::unique_ptr<UIManager> UIManager_;

};

