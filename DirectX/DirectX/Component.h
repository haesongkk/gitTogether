#pragma once
class GameObject;
#include "Base.h"
class Component abstract : public ObjectBase
{
public:
	Component(weak_ptr<GameObject> _wOwnerObj) { }
	virtual ~Component() { }
	virtual void Run() { }
};