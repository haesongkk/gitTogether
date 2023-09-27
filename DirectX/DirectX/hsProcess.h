#pragma once
#include "hsBase.h"
class hsSystem;
class hsProcess : public hsBase
{
public:
	hsProcess(HINSTANCE _hInst, UINT _width, UINT _height);

	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;

	template<typename T>
	T* GetSystem()
	{
		for (auto pSys : m_pSystems)
			if (dynamic_cast<T*>(pSys))
				return dynamic_cast<T*>(pSys);
		return nullptr;
	}

private:
	vector<hsSystem*> m_pSystems;

};

