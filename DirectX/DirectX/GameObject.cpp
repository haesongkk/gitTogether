#include "framework.h"
#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "SkinnedAnimation.h"

#include "State.h"

shared_ptr<Component> GameObject::GetComponent(ComponentType ct)
{
	assert(ct < m_spComponents.size());
	return m_spComponents[ct];
}

GameObject::~GameObject()
{
	m_spComponents.clear();
	m_spStates.clear();
}

void GameObject::Create(string name, RenderType rt, PhysicsType pt)
{
	m_name = name;
	m_spComponents.resize(CT_End, nullptr);
	m_spComponents[CT_Transform] = make_shared<Transform>();
	switch (rt)
	{
		case RT_SkinnedAnimation:
			m_spComponents[CT_Render] = make_shared<SkinnedAnimation>();
			break;
	}
	switch (pt)
	{
	}
}

void GameObject::Run()
{
	for (auto spComp : m_spComponents)
		spComp->Run();
	for (auto spState : m_spStates)
		spState->Run();
}