#pragma once
class GameObject;
class Component abstract
{
public:
	// ������
	Component(weak_ptr<GameObject> _wOwnerObj) { }

public:
	// �Ҹ���
	virtual ~Component() { }

public:
	// ������Ʈ
	virtual void Run() { }

};