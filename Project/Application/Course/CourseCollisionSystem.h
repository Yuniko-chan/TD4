#pragma once
#include "../../Engine/Object/MeshObject.h"

/// <summary>
/// コース衝突システム
/// </summary>
class CourseCollisionSystem
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 実行
	/// </summary>
	void Execute();

	/// <summary>
	/// コースとぶつかるオブジェクト登録
	/// </summary>
	/// <param name="object">オブジェクトのポインタ</param>
	void ObjectRegistration(MeshObject* object);

private:
	
	/// <summary>
	/// 距離判定
	/// </summary>
	/// <param name="object">オブジェクト</param>
	void DistanceJudgment(MeshObject* object);

private: // メンバ変数

	// コース

	// コースとぶつかるオブジェクト
	std::list<MeshObject*> collidingObjects_;


};

