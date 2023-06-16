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
	// 로컬좌표계에 출력을 하며, Transform이 적용될 것을 고려한 구현
	IDREngine::GetInstance()->DrawText(0, -20, "Component : %s", m_Name.c_str());
}

void Traditional::Renderer::OnPostRender()
{

}

