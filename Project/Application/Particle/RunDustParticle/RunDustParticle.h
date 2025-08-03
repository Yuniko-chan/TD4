#pragma once
#include "../../../Application/Particle/GPUParticle3D/GPUParticle3D.h"
#pragma once
#include "../../../Engine/GPUParticle/GPUParticle.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/Model/ModelManager.h"
#include "../../../Engine/base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/base/BufferResource.h"
#include "../../../Engine/GPUParticle/ParticleCS.h"
#include "../../../Engine/base/CompileShader.h"
#include "../../../Engine/base/DxCommon/Log.h"
#include "../../../Engine/Particle/BillBoardMatrix.h"
class RunDustParticle :
	public GPUParticle
{

public:

	struct ParticleBlendNormalCS
	{

		Vector3 translate; // 位置
		Vector3 scale; // 大きさ
		float lifeTime; // 生存時間
		Vector3 velocity; // 速度
		float currentTime; // 経過時間
		Vector4 color; // 色
		Vector3 rotate;
		Vector3 rotateVelocity;
		Matrix4x4 rotateMatrix;
	};

	struct EmitBlendNormalCS
	{

		Vector3 translate0; // 位置
		float pad0;
		Vector3 translate1; // 位置
		float pad1;
		Vector3 translate2; // 位置
		float pad2;
		Vector3 translate3; // 位置

		uint32_t num;

		float radius; // 射出半径
		uint32_t count; // 射出数
		float frequency; // 射出間隔
		float frequencyTime; // 射出間隔調整時間
		uint32_t emit; // 射出許可

	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootSignature">ルートシグネチャ</param>
	/// <param name="pipelineState">パイプラインステート</param>
	void Initialize(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState* pipelineState,
		const std::string& name = "Particle") override;

	/// <summary>
	/// 
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(
		ID3D12GraphicsCommandList* commandList,
		BaseCamera& camera) override;

	/// <summary>
	/// エミッタ情報設定
	/// </summary>
	/// <param name="emitter">エミッター</param>
	/// <param name="isEmitSet">エミッター情報をセットするか</param>
	void SetEmitter(const EmitBlendNormalCS& emitter, bool isEmitSet = true);

private:

	/// <summary>
	/// バッファの初期化
	/// </summary>
	/// <param name="device"></param>
	void UAVBufferInitialize(ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList) override;

	/// <summary>
	/// 定数バッファ初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void ConstantBufferInitialzie(ID3D12Device* device) override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void InitialzieCS(ID3D12GraphicsCommandList* commandList) override;

	/// <summary>
	/// エミット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void Emit(ID3D12GraphicsCommandList* commandList) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateCS(ID3D12GraphicsCommandList* commandList) override;

private: // パイプラインステートの初期化CS

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForInitialize(ID3D12Device* device) override;

	/// <summary>
	/// エミット
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForEmit(ID3D12Device* device) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForUpdate(ID3D12Device* device) override;

private:

	// 卵モデル
	Model* eggModel_;

	// 
	EmitBlendNormalCS* emitBlendNormalMap_;

};


