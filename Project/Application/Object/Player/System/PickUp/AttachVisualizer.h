#pragma once
#include "BaseInteractionVisualizer.h"

#include "../../../Utility/Math/Vector2Int.h"
#include "../../../Engine/Math/Vector/Vector3.h"

#include <map>
#include <string>

class InteractionSpot;

class AttachVisualizer : public BaseInteractionVisualizer
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="owner"></param>
	void Initialize(Player* owner) override;
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="key"></param>
	void Update(const Vector2Int& key);

};