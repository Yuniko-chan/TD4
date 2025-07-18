#pragma once
#include "../../../Utility/Common/OwnerComponent.h"
#include "../../../Engine/3D/Transform/WorldTransform.h"

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

	/// <summary>
	/// リフレッシュ
	/// </summary>
	virtual void Refresh();

protected:
	// インタラクト用のオブジェクト
	InteractionSpot* InteractObject_ = nullptr;
	// 親のトランスフォーム
	WorldTransform* parent_ = nullptr;

};
