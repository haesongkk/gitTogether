#include "CircleRenderer.h"
#include "IDREngine.h"
#include <typeinfo>


Traditional::CircleRenderer::CircleRenderer()
	: Renderer(), m_Radius(1), m_AABB()
{

	m_Name = typeid(*this).name();
}

Traditional::CircleRenderer::~CircleRenderer()
{

}

void Traditional::CircleRenderer::Update()
{

}

void Traditional::CircleRenderer::OnPreRender()
{

}

void Traditional::CircleRenderer::OnRender()
{
	// 로컬좌표계에 출력을 하며, Transform이 적용될 것을 고려한 구현
	IDREngine::GetInstance()->DrawText(0, -20, "Component : %s", m_Name.c_str());

	// 일단 원을 그린다.
	IDREngine::GetInstance()->DrawEllipse(0, 0, (int)m_Radius);
}

void Traditional::CircleRenderer::OnPostRender()
{
	DrawDebugAxes();
}

void Traditional::CircleRenderer::DrawDebugAxes()
{
	// y축은 녹색
	IDREngine::GetInstance()->SetBrushColor(0.f, 1.f, 0.f, 1.f);
	IDREngine::GetInstance()->DrawLine(0, 0, 0, -100);

	// x축은 빨간색
	IDREngine::GetInstance()->SetBrushColor(1.f, 0.f, 0.f, 1.f);
	IDREngine::GetInstance()->DrawLine(0, 0, 100, 0);

	IDREngine::GetInstance()->SetBrushColor(1.f, 1.f, 1.f, 1.f);
}

