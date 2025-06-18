#pragma once
#include "../../../Engine/3D/Model/BaseInstancingDrawing.h"

/// <summary>
/// ゲームシーン用インスタンシング描画クラス
/// </summary>
class GameSceneInstancingDrawing :
    public BaseInstancingDrawing
{

public: // 関数

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 登録確認
    /// </summary>
    /// <param name="meshObject">メッシュオブジェクト</param>
    /// <param name="viewProjectionMatrix">ビュープロジェクション</param>
    /// <returns>成功したか</returns>
    bool RegistrationConfirmation(
        MeshObject* meshObject,
        const Matrix4x4& viewProjectionMatrix) override;

private: // 関数

    /// <summary>
    /// メッシュオブジェクト
    /// </summary>
    /// <param name="meshObject">オブジェクト</param>
    /// <param name="viewProjectionMatrix">ビュープロジェクション</param>
    void CannonVer(MeshObject* meshObject, const Matrix4x4& viewProjectionMatrix);

    /// <summary>
    /// 鉄球
    /// </summary>
    /// <param name="meshObject">オブジェクト</param>
    /// <param name="viewProjectionMatrix">ビュープロジェクション</param>
    void ConicalPendulumIronBallVer(MeshObject* meshObject, const Matrix4x4& viewProjectionMatrix);

    /// <summary>
    /// 鉄球
    /// </summary>
    /// <param name="meshObject">オブジェクト</param>
    /// <param name="viewProjectionMatrix">ビュープロジェクション</param>
    void PendulumIronBallVer(MeshObject* meshObject, const Matrix4x4& viewProjectionMatrix);

    /// <summary>
    /// ミニガン
    /// </summary>
    /// <param name="meshObject">オブジェクト</param>
    /// <param name="viewProjectionMatrix">ビュープロジェクション</param>
    void MinigunVer(MeshObject* meshObject, const Matrix4x4& viewProjectionMatrix);

private: // 関数

    void RopeRegistrationConfirmation(WorldTransform* worldTransform, const MaterialData& materialData, const Matrix4x4& viewProjectionMatrix);

};

