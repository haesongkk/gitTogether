#pragma once
#include "hsSystem.h"
class hsObject;
class hsWorld : public hsSystem
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;

private:
	vector<hsObject*> m_pObjects;
};

