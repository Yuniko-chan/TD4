#pragma once

/// <summary>
/// シングルトンの継承用
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class SingletonModule
{
public:
	// インスタンス取得
	static T* GetInstance() {
		static T instance;
		return &instance;
	}
protected:
	SingletonModule() = default;
	virtual ~SingletonModule() = default;

	// コピー禁止
	SingletonModule(const SingletonModule&) = delete;
	const SingletonModule& operator=(const SingletonModule&) = delete;

};
