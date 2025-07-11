#pragma once
#include "BaseInteractionVisualizer.h"
#include <map>

class InteractionSpot;

class PickupVisualizer : public BaseInteractionVisualizer
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="owner"></param>
	void Initialize(Player* owner) override;

	/// <summary>
	/// スポット追加
	/// </summary>
	/// <param name="name"></param>
	/// <param name="interact"></param>
	void AddSpot(std::string name, InteractionSpot* interact);
private:
	/// <summary>
	/// スポット検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	InteractionSpot* FindSpot(const std::string& name);
public:
	/// <summary>
	/// スポット更新
	/// </summary>
	/// <param name="name"></param>
	void RefrashSpot(const std::string& name);
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();
	/// <summary>
	/// セットアップ
	/// </summary>
	/// <param name="position"></param>
	/// <param name="direction"></param>
	void SetUp(const Vector3& position, const Vector3& direction);
private:
	// スポット配列
	std::map<std::string, InteractionSpot*> interactionSpots_;
	// インタラクト用のオブジェクト
	InteractionSpot* InteractObject_ = nullptr;

};