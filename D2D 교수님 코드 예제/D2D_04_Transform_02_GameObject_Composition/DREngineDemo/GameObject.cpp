#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "CircleRenderer.h"


Traditional::GameObject::GameObject(string name)
	: m_Name(name), m_pTransform(nullptr), m_pRenderer(nullptr)
{

}

Traditional::GameObject::~GameObject()
{
	delete m_pRenderer;
	delete m_pTransform;
}

void Traditional::GameObject::Update()
{
	m_pTransform->Update();
	m_pRenderer->Update();
}

void Traditional::GameObject::Render()
{
	m_pTransform->OnPreRender();
	m_pRenderer->OnPreRender();

	m_pTransform->OnRender();
	m_pRenderer->OnRender();

	m_pTransform->OnPostRender();
	m_pRenderer->OnPostRender();
}

void Traditional::GameObject::CreateTransform(CVECTOR2 position, float angle, CVECTOR2 scale)
{
	// *기존의 Transform이 있는가 없는가 체크, 중복을 막거나 기존 것을 delete해야 한다.

	m_pTransform = new Transform();

	m_pTransform->SetLocalPosition(position);
	m_pTransform->SetLocalRotation(angle);
	m_pTransform->SetLocalScale(scale);
}

void Traditional::GameObject::CreateCircleRenderer(float radius)
{
	m_pRenderer = new CircleRenderer();

	m_pRenderer->m_Radius = radius;
}

void Traditional::GameObject::AttachTransform(Transform* val)
{
	m_pTransform = val;
	val->m_pGameObject = this;
}

void Traditional::GameObject::AttachRenderer(CircleRenderer* val)
{
	m_pRenderer = val;
}

