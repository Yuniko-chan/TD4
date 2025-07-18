#pragma once
#include "BaseInteractionVisualizer.h"
#include "../../../Engine/3D/Transform/WorldTransform.h"

class PickupVisualizer : public BaseInteractionVisualizer
{
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//---アクセッサ---//
	// 設定
	void SetSpot(InteractionSpot* spot) { this->InteractObject_ = spot; }
	void SetTransform(WorldTransform* parent) { parent_ = parent; }
	void SetIsDraw(bool isDraw);
	// 取得
	bool IsParent() { return parent_; }

};
