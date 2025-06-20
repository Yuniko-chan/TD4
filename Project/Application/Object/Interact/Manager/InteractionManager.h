#pragma once
#include <vector>

class InteractionSpot;
class BaseObjectManager;

class InteractionManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	//void Initialize();

public: // アクセッサ
	// オブジェクトマネージャ
	void SetObjectManager(BaseObjectManager* objectManager) { objectManager_ = objectManager; }

private:
	// オブジェクトのプール
	std::vector<bool> pool_;
	// 
	std::vector<InteractionSpot*> spots_;
	// オブジェクトマネージャー
	BaseObjectManager* objectManager_ = nullptr;

};