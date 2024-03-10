#pragma once
class Component;
class State;
#include "Base.h"
class GameObject : public ObjectBase
{
public:
	enum ComponentType
	{
		CT_Transform,
		CT_Render,
		CT_Physics,
		CT_End,
	};

	enum RenderType
	{
		RT_SkinnedAnimation,
		RT_None,
	};

	enum PhysicsType
	{
		eNone,
	};

private:
	string m_name;
	spVector<Component> m_spComponents;
	spVector<State> m_spStates;
	
public:
	shared_ptr<Component> GetComponent(ComponentType ct);

public:
	void Create(string name, RenderType rt, PhysicsType pt);
	virtual void Run();

};

