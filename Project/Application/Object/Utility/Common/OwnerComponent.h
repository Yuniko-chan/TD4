#pragma once

template <typename T>
class OwnerComponent {
public:
	/// <summary>
	/// オーナー（所持者）の設定
	/// </summary>
	/// <param name="owner"></param>
	void SetOwner(T* owner) { owner_ = owner; }
protected:
	// 所持先
	T* owner_ = nullptr;
};
