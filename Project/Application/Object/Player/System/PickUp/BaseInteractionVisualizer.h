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

	/// <summary>
	/// 描画フラグ
	/// </summary>
	/// <param name="isDraw"></param>
	virtual void SetIsDraw(bool isDraw);

	InteractionSpot* GetInteract() { return interactObject_; }
public:
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
	virtual void RefrashSpot(const std::string& name);
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
protected:
	/// <summary>
	/// スポット検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	InteractionSpot* FindSpot(const std::string& name);
	// スポット配列
	std::map<std::string, InteractionSpot*> interactionSpots_;

protected:
	// インタラクト用のオブジェクト
	InteractionSpot* interactObject_ = nullptr;
	// 親のトランスフォーム
	WorldTransform* parent_ = nullptr;
	// 描画
	bool isDraw_ = false;
};
