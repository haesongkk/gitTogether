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
	// 로컬좌표계에 출력을 하며, Transform이 적용될 것을 고려한 구현
	IDREngine::GetInstance()->DrawText(0, -20.f, "Component : %s", name.c_str());
}

void CBEngine::Renderer::OnPostRender()
{

}

