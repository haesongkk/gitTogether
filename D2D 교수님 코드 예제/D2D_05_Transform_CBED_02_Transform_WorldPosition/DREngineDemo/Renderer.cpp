#include "Renderer.h"
#include "IDREngine.h"


CBEngine::Renderer::Renderer(std::string name)
{

}

CBEngine::Renderer::~Renderer()
{

}

void CBEngine::Renderer::Update()
{

}

void CBEngine::Renderer::OnPreRender()
{

}

void CBEngine::Renderer::OnRender()
{
	// ������ǥ�迡 ����� �ϸ�, Transform�� ����� ���� ����� ����
	IDREngine::GetInstance()->DrawText(0, -20.f, "Component : %s", name.c_str());
}

void CBEngine::Renderer::OnPostRender()
{

}

