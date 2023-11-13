#pragma once
template <class Manager>
class SingletonBase abstract
{
	SingletonBase() = default;
	~SingletonBase() = default;

public:
	static unique_ptr<Manager> GetSingle()
	{
		static unique_ptr<Manager> mgr = make_unique<Manager>();
		return mgr;
	}
};

class ObjectBase abstract
{
public:
	ObjectBase() = default;
	~ObjectBase() = default;

};

