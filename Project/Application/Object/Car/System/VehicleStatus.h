#pragma once
#include "../../Utility/Common/OwnerComponent.h"
#include "../../Utility/Math/Vector2Int.h"
#include "../../../Engine/Math/Vector/Vector4.h"

#include <string>
#include <map>

namespace Car { class IParts; }
class VehicleCore;

class VehicleStatus : public OwnerComponent<VehicleCore>
{
private:
	struct PartsType
	{
		int16_t tire;
		int16_t armor;
		int16_t engine;
	};
	struct DirectionType {
		int16_t forward;
		int16_t backForward;
		int16_t left;
		int16_t right;
	};

public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// ステータスの更新
	/// </summary>
	/// <param name="lists"></param>
	void StatusUpdate(std::map<Vector2Int, Car::IParts*>* lists);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
public: // 適応させる処理
	// パーツ追加適応
	void ApplyPartAdd(std::string name, Vector2Int key);
	// パーツ削除適応
	void ApplyPartRemove(std::string name, Vector2Int key);

public:	// アクセッサ
	float GetSpeed() const { return speed_; }
	float GetWeight() const { return weight_; }
	float GetArmor() const { return armor_; }
	// ホイールの取得
	int GetLeftWheel() const { return (int)wheelDirectCount_.x; }
	int GetRightWheel() const { return (int)wheelDirectCount_.y; }
	// タイヤの数
	int GetTire() const { return partsTypes_.tire; }
	// エンジン数
	int GetEngine() const { return partsTypes_.engine; }
	// オーバーヒートのチェック
	bool GetIsOverheat() const { return isOverheat_; }
	float GetHeatDPS() const { return damagePerSecond_; }

	//---セッター---//
	void SetIsOverheat(bool isOverheat) { isOverheat_ = isOverheat; }
	void SetDamagePerSecond(float damage) { damagePerSecond_ = damage; }
private:
	float weight_ = 1.0f;
	float speed_ = 1.0f;
	float armor_ = 1.0f;
	// パーツの数
	PartsType partsTypes_;
	// パーツの方角ごとの数
	DirectionType directionTypes_;
	// 回転用の数（X:左,Y：右）
	Vector2Int handlingWheel_ = {};
	// X:左,Y:右,Z:前,W:後
	Vector4 wheelDirectCount_ = {};

	// オーバーヒート関係
	bool isOverheat_ = false;
	float damagePerSecond_ = 1.0f;
};
