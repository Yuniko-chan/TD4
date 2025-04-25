#pragma once
#include "../../../Engine/Object/MeshObject.h"

/// <summary>
/// コースデモ
/// </summary>
class CourseDemoObject :
    public MeshObject
{

public:

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// ImGui描画
    /// </summary>
    /// <param name="camera"></param>
    void ImGuiDraw() override;

};

