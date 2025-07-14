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
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="key"></param>
	void Update(const Vector2Int& key);
private:
	// スポット配列
	std::map<std::string, InteractionSpot*> interactionSpots_;

};