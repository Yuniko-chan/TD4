#pragma once
#include <stdint.h>
#include "../Utility/Common/SingletonModule.h"

class VehicleCore;

/// <summary>
/// チュートリアル
/// </summary>
class Tutorial : public SingletonModule<Tutorial>
{
public:
	Tutorial();
	~Tutorial() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// コア設定
	/// </summary>
	void SetVehicleCore(VehicleCore* vehicleCore) { vehicleCore_ = vehicleCore; }
	void SetCourseTraversalNum(int32_t* courseTraversalNum) { courseTraversalNum_ = courseTraversalNum; }

	bool GetIsEnd() { return isEnd_; }
	bool GetEngineCheck() { return engineCheck_; }
	bool GetTireCheck() { return tireCheck_; }
	bool GetFrameCheck() { return frameCheck_; }
	bool IsDrive() { return isDrive_; }

private: // メンバ変数

	// 終了したか
	bool isEnd_;

	// エンジン確認
	bool engineCheck_;

	// タイヤ確認
	bool tireCheck_;
	
	// フレーム確認
	bool frameCheck_;

	// コア
	VehicleCore* vehicleCore_;

	// 踏破数
	int32_t* courseTraversalNum_;

	// 運転中か？
	bool isDrive_;

};

