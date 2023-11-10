#include "framework.h"
#include "GameObject.h"

#include "Component.h"

GameObject::GameObject()
	:m_spComponents{} { }

GameObject::~GameObject()
{
	m_spComponents.clear();
}

void GameObject::Run()
{
	for (auto sComp : m_spComponents)
		sComp->Run();
}
