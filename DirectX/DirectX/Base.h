#pragma once
template<class T>
class SingletonBase abstract
{
protected:
	SingletonBase() = default;
	virtual ~SingletonBase() = default;

public:
	static unique_ptr<T>& Get()
	{
		static unique_ptr<T> _instance = make_unique<T>();
		return _instance;
	}
};

class ObjectBase abstract
{
public:
	ObjectBase() = default;
	virtual ~ObjectBase() = default;
	virtual void Run() { }

};