#pragma once

template <typename T>
class OwnerComponent {
public:
	/// <summary>
	/// オーナー（所持者）のセッター
	/// </summary>
	/// <param name="owner"></param>
	void SetOwner(T* owner) { owner_ = owner; }
	/// <summary>
	/// 所持者のゲッター
	/// </summary>
	/// <returns></returns>
	T* GetOwner() { return owner_; }

protected:
	// 所持先
	T* owner_ = nullptr;
};
