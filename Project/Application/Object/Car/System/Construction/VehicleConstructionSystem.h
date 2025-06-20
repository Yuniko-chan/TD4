#pragma once
#include "../VehicleSystemCommons.h"
#include <map>
#include <utility>

namespace Car { class IParts; }
class VehicleCore;
class VehicleStatus;

/// <summary>
/// 車両構築システム（コア用）（パーツ管理など）
/// </summary>
class VehicleConstructionSystem : public OwnerComponent<VehicleCore>
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="core"></param>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 方向判断まで
	/// </summary>
	/// <param name="parts"></param>
	bool Attach(Car::IParts* parts);

	/// <summary>
	/// 解除
	/// </summary>
	/// <param name="parts"></param>
	void Detach(Car::IParts* parts);

	/// <summary>
	/// 任意指定でのくっつけ処理
	/// </summary>
	/// <param name="parts"></param>
	/// <param name="key"></param>
	void AnyDocking(Car::IParts* parts, const Vector2Int& key);

	/// <summary>
	/// 近いパーツの取得
	/// </summary>
	/// <param name="point"></param>
	/// <returns></returns>
	Car::IParts* FindNearPart(const Vector3& point);

	std::map<Vector2Int, Car::IParts*>* GetPartMappingPtr() { return &partsMapping_; }

private: // 指定して設定OR解除
	/// <summary>
	/// 接続
	/// </summary>
	/// <param name="parts"></param>
	void Attach(Car::IParts* parts, const Vector2Int& key);
	/// <summary>
	/// 解除
	/// </summary>
	/// <param name="it"></param>
	void Detach(std::map<Vector2Int, Car::IParts*>::iterator it);

private: // 検索
	/// <summary>
	/// 検索
	/// </summary>
	/// <param name="directLists"></param>
	/// <param name="number"></param>
	/// <returns></returns>
	Car::IParts* FindPreNumber(std::list<std::pair<int, Car::IParts*>>* directLists, int32_t number);

	/// <summary>
	/// ポインタから検索
	/// </summary>
	/// <param name="parts"></param>
	/// <returns></returns>
	Car::IParts* FindParts(Car::IParts* parts);

public:
	std::vector<Car::IParts*> FindPartsByCategory(int typeID);

private: // 登録・解除
	/// <summary>
	/// パーツの登録処理
	/// </summary>
	/// <param name="parts"></param>
	void RegistParts(const Vector2Int& id, Car::IParts* parts);
	/// <summary>
	/// パーツの解除処理
	/// </summary>
	/// <param name="id"></param>
	/// <param name="parts"></param>
	void UnRegistParts(const Vector2Int& id, Car::IParts* parts);

private:
	// ステータス
	VehicleStatus* status_ = nullptr;
	// パーツのリスト
	std::map<Vector2Int, Car::IParts*> partsMapping_;
	// 空いてるキーリスト
	std::vector<Vector2Int> emptyMap_;
public: // アクセッサ
	void SetStatusManager(VehicleStatus* status) { status_ = status; }

	bool IsEmpty() { return partsMapping_.empty(); }
};
