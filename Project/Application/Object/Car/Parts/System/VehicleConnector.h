#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Utility/Common/OwnerComponent.h"
#include "../../../Engine/Math/Vector/Vector2.h"
#include <list>
#include <array>

// 前方宣言
namespace Car { class IParts; }

class VehicleConnector : public OwnerComponent<Car::IParts>
{
public:
	~VehicleConnector() = default;
	VehicleConnector() {};
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// リセット
	/// </summary>
	void Reset() {
		parents_.clear();
		childrens_.clear();
	}
	// 親の追加
	void AddParents(Car::IParts* part) {
		parents_.push_back(part); 
		parents_.unique();
	}
	// 子の追加
	void AddChildren(Car::IParts* part) {
		childrens_.push_back(part);
		childrens_.unique();
	}
	// 親の削除
	void DeleteParent(Car::IParts* part) { parents_.remove(part); }
	void ReleaseParent(Car::IParts* part);
	// 子の削除
	void DeleteChildren(Car::IParts* part) { childrens_.remove(part); }
	void ReleaseChildren(Car::IParts* part);
public:	// アクセッサ
	// 深度値取得
	int32_t GetDepth() const { return depth_; }
	// 深度地設定
	void SetDepth(int32_t depth) { depth_ = depth; }
	void SetKey(const Vector2& key) { debugKey_ = key; }
	// 親があるか
	bool IsParent() { return !parents_.empty(); }

	/// <summary>
	/// 子の整理
	/// </summary>
	void OrganizeChildren();

private:
	// 隣接パーツ
	std::list<Car::IParts*> parents_;
	std::list<Car::IParts*> childrens_;
	// 深度値
	int32_t depth_ = 0;
	// デバッグ用
	Vector2 debugKey_ = {};
	// 親ポインタ（座標構築用）
	WorldTransform* coreTransform_ = nullptr;
};
