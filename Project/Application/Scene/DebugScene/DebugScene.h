#pragma once

#include "../../../Engine/Scene/BaseScene/BaseScene.h"
#include "../../Object/Skydome/Skydome.h"
#include "../../../Engine/Physics/Cloth/Cloth.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"
#include "../../ClothDemo/ClothDemo.h"
#include "../../Course/Course.h"
#include "../../Course/CourseCollisionSystem.h"

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

	// クロスシミュレーションデモ
	std::unique_ptr<ClothDemo> clothDemo_;

	// Course
	std::unique_ptr<Course> course_;
	std::unique_ptr<Model> courseModel_;

	// CourseCollisionSystem 
	std::unique_ptr<CourseCollisionSystem> courseCollisionSystem_;

};

