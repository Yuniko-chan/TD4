#pragma once
#include "../../BaseCharacter/BaseCharacterAnimation.h"

/// <summary>
/// ミニガンアニメーション
/// </summary>
class MinigunAnimation :
    public BaseCharacterAnimation
{

public: // 関数
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model) override;

};

