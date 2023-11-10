#pragma once
class GameObject;
class Component abstract
{
public:
	// 생성자
	Component(weak_ptr<GameObject> _wOwnerObj) { }

public:
	// 소멸자
	virtual ~Component() { }

public:
	// 업데이트
	virtual void Run() { }

};