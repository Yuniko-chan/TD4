#pragma once
#include "../../../Utility/Common/OwnerComponent.h"
#include "../../../Engine/Object/MeshObject.h"

class Player;

class BaseInteractionVisualizer : public OwnerComponent<Player>
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="owner"></param>
	virtual void Initialize(Player* owner);

private:


};
