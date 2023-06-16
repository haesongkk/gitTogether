#include "Renderer.h"
#include "IDREngine.h"
#include <typeinfo>


Traditional::Renderer::Renderer()
{
	m_Name = typeid(*this).name();
}

Traditional::Renderer::~Renderer()
{

}

void Traditional::Renderer::Update()
{

}

void Traditional::Renderer::OnPreRender()
{

}

void Traditional::Renderer::OnRender()
{
	// ������ǥ�迡 ����� �ϸ�, Transform�� ����� ���� ����� ����
	IDREngine::GetInstance()->DrawText(0, -20, "Component : %s", m_Name.c_str());
}

void Traditional::Renderer::OnPostRender()
{

}

