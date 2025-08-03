#pragma once
#include "../GPUParticle3D/GPUParticle3D.h"
#include "../RunDustParticle/RunDustParticle.h"
class RunDustManager
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(BaseCamera& camera);

	/// <summary>
	/// 位置登録
	/// </summary>
	/// <param name="position"></param>
	void PositionRegister(const Vector3& position);

	/// <summary>
	/// 位置
	/// </summary>
	void PositionClear();

	/// <summary>
	/// 
	/// </summary>
	void StopEmit() {

		emitNum_ = 0;
	}

private:

	static const uint32_t kEmitNumMax = 4;

	// パーティクル
	std::unique_ptr<RunDustParticle> runDustParticle_;

	// 発生位置
	std::array<Vector3, kEmitNumMax> emitPositions_;

	// 現在の発生数
	uint32_t emitNum_;

	// DirectXCommon
	DirectXCommon* dxCommon_;

};

