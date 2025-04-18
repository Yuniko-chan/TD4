#pragma once

class Player;

/// <summary>
/// 基底コンポネント
/// </summary>
class PlayerComponent
{
public:
	void SetPlayer(Player* player) { player_ = player; }
protected:
	Player* player_ = nullptr;

};
