#pragma once
class GameObject;
class Component 
{
protected:
	weak_ptr<GameObject> m_wOwnerObj;

public:
	// ������
	Component(weak_ptr<GameObject> _wOwnerObj)
		:m_wOwnerObj(_wOwnerObj) { }

public:
	// �Ҹ���
	~Component() { m_wOwnerObj.reset(); }

public:
	// ������Ʈ
	virtual void Run() abstract { }

};

