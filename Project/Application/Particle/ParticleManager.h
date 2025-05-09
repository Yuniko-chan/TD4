#pragma once
#include "../../../Engine/GPUParticle/GPUParticle.h"

class ParticleManager {
public:
	// テクスチャ一覧
	enum TextureIndex {
		kTextureIndexOfCount
	};

	// パーティクル一覧
	enum ParticleIndex {
		kUIIndexOfCount
	};

public:
	/// <summary>
	///	初期化
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
	/// Imgui描画
	/// </summary>
	void ImGuiDraw();

private:
	std::array<std::unique_ptr<GPUParticle>, ParticleIndex::kUIIndexOfCount> particles_;
};