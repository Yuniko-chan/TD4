#pragma once
#include "../../Utility/Common/OwnerComponent.h"
#include "../../Utility/Math/Vector2Int.h"

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
	void Update();
	void StatusUpdate(std::map<Vector2Int, Car::IParts*>* lists);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
public:
	void ApplyPartAdd(std::string name, Vector2Int key);
	void ApplyPartRemove(std::string name, Vector2Int key);

public:	// アクセッサ
	float GetSpeed() const { return speed_; }
	float GetWeight() const { return weight_; }
	float GetArmor() const { return armor_; }
	// ホイールの取得
	int GetLeftWheel() const { return wheelLeft_; }
	int GetRightWheel() const { return wheelRight_; }
	// タイヤの数
	int GetTire() const { return partsTypes_.tire; }
	// エンジン数
	int GetEngine() const { return partsTypes_.engine; }
	// オーバーヒートのチェック
	bool GetIsOverheat() const { return isOverheat_; }

	//---セッター---//
	void SetIsOverheat(bool isOverheat) { isOverheat_ = isOverheat; }

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
	int wheelLeft_ = 0;	// 左
	int wheelRight_ = 0;	// 右
	int wheelReal_ = 0;	// 後方
	int wheelFront_ = 0;	// 前方

	bool isOverheat_ = false;
};
