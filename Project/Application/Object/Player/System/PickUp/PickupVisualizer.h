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

public: //---アクセッサ---//
	// 設定
	void SetSpot(InteractionSpot* spot) { this->interactObject_ = spot; }
	void SetTransform(WorldTransform* parent) { parent_ = parent; }
	// 取得
	bool IsParent() { return parent_; }

public: // メンバ関数
	/// <summary>
	/// スポット更新
	/// </summary>
	/// <param name="name"></param>
	void RefrashSpot(const std::string& name) override;

};
