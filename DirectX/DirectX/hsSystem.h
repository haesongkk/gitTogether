#pragma once
#include "hsBase.h"
class hsProcess;
class hsSystem abstract: public hsBase
{
public: 
	virtual void Init() abstract { }
	virtual void Update() abstract { }
	virtual void Finalize() abstract { }

};

