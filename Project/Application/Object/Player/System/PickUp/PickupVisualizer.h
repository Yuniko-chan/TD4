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
	/// スポット検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	InteractionSpot* FindSpot(const std::string& name);
	/// <summary>
	/// スポット追加
	/// </summary>
	/// <param name="name"></param>
	/// <param name="interact"></param>
	void AddSpot(std::string name, InteractionSpot* interact);
	/// <summary>
	/// スポット更新
	/// </summary>
	/// <param name="name"></param>
	void RefrashSpot(const std::string& name);
	/// <summary>
	/// セットアップ
	/// </summary>
	/// <param name="position"></param>
	/// <param name="direction"></param>
	void SetUp(const Vector3& position, const Vector3& direction);
private:
	// スポット配列
	std::map<std::string, InteractionSpot*> interactionSpots_;

};
