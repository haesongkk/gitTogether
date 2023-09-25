#pragma once
#include "hsBase.h"
class hsComponent;
class hsObject : public hsBase
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;

public:
	string& GetName() { return mName; }

	template<typename T>
	T* AddComponent()
	{
		assert(is_base_of<Component, T>::value);
		T* pComp = new T(this);
		mComps.push_back(pComp);
	}

	template<typename T>
	T* GetComponent()
	{
		for (auto pComp : mComps)
			if (dynamic_cast<T*>(pComp))
				return pComp;
		return nullptr;
	}

private:
	string mName;
	vector<hsComponent*> mComps;

};

