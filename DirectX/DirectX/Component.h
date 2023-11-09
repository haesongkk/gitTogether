#pragma once
class GameObject;
class Component 
{
protected:
	weak_ptr<GameObject> m_wOwnerObj;

public:
	// 생성자
	Component(weak_ptr<GameObject> _wOwnerObj)
		:m_wOwnerObj(_wOwnerObj) { }

public:
	// 소멸자
	~Component() { m_wOwnerObj.reset(); }

public:
	// 업데이트
	virtual void Run() abstract { }

};

