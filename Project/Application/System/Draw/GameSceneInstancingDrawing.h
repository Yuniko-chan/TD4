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

};

