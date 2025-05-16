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
#include "../../Object/Player/Player.h"

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

	// 円錐振り子
	std::unique_ptr<PendulumIronBall> pendulumIronBall_;
	std::unique_ptr<Model> pendulumIronBallModel_;

	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;

};

