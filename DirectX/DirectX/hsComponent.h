#pragma once
#include "hsBase.h"
class hsObject;
class hsComponent : public hsBase
{
public:
	hsComponent(hsObject* _pOwner)
		: m_pOwner{_pOwner} { }

	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Finalize() abstract;

private:
	hsObject* m_pOwner;

};