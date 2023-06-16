#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "PrimitiveRenderer2D.h"


CBEngine::GameObject::GameObject(string name)
	: m_Name(name)
{

}

CBEngine::GameObject::~GameObject()
{

}

void CBEngine::GameObject::Update()
{
	for (Component* co : m_Components)
	{
		co->Update();
	}
}

void CBEngine::GameObject::Render()
{
	for (Component* co : m_Components)
	{
		// Transform의 SetTransform() 같은 것
		co->OnPreRender();
	}

	for (Component* co : m_Components)
	{
		co->OnRender();
	}

	for (Component* co : m_Components)
	{
		co->OnPostRender();
	}
}
