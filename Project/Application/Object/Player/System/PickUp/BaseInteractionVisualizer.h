#pragma once
#include "../../../Utility/Common/OwnerComponent.h"

class Player;
class InteractionSpot;

class BaseInteractionVisualizer : public OwnerComponent<Player>
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="owner"></param>
	virtual void Initialize(Player* owner);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

protected:
	// インタラクト用のオブジェクト
	InteractionSpot* InteractObject_ = nullptr;
};
