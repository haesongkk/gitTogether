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
	// ������ǥ�迡 ����� �ϸ�, Transform�� ����� ���� ����� ����
	IDREngine::GetInstance()->DrawText(0, -20, "Component : %s", m_Name.c_str());

	// �ϴ� ���� �׸���.
	IDREngine::GetInstance()->DrawEllipse(0, 0, (int)m_Radius);
}

void Traditional::CircleRenderer::OnPostRender()
{
	DrawDebugAxes();
}

void Traditional::CircleRenderer::DrawDebugAxes()
{
	// y���� ���
	IDREngine::GetInstance()->SetBrushColor(0.f, 1.f, 0.f, 1.f);
	IDREngine::GetInstance()->DrawLine(0, 0, 0, -100);

	// x���� ������
	IDREngine::GetInstance()->SetBrushColor(1.f, 0.f, 0.f, 1.f);
	IDREngine::GetInstance()->DrawLine(0, 0, 100, 0);

	IDREngine::GetInstance()->SetBrushColor(1.f, 1.f, 1.f, 1.f);
}

