#include "PrimitiveRenderer2D.h"
#include "IDREngine.h"

CBEngine::PrimitiveRenderer2D::PrimitiveRenderer2D()
	: m_Radius(1), m_AABB()
{

	name = typeid(*this).name();
}

CBEngine::PrimitiveRenderer2D::~PrimitiveRenderer2D()
{

}

void CBEngine::PrimitiveRenderer2D::Update()
{

}

void CBEngine::PrimitiveRenderer2D::OnPreRender()
{

}

void CBEngine::PrimitiveRenderer2D::OnRender()
{
	// ������ǥ�迡 ����� �ϸ�, Transform�� ����� ���� ����� ����
	IDREngine::GetInstance()->DrawText(0, -20.f, "Component : %s", name.c_str());

	switch (primitiveType)
	{
		case eCollisionType::eCC:
		{
			IDREngine::GetInstance()->DrawEllipse(0, 0, m_Radius);
		}
		break;

		case eCollisionType::eAABB:
		{
			//IDREngine::GetInstance()->DrawRectangle()
		}
		break;

		default:
		{

		}
		break;
	}
}

void CBEngine::PrimitiveRenderer2D::OnPostRender()
{
	DrawDebugAxes();
}

void CBEngine::PrimitiveRenderer2D::DrawDebugAxes()
{
	// y���� ���
	IDREngine::GetInstance()->SetBrushColor(0.f, 1.f, 0.f, 1.f);
	IDREngine::GetInstance()->DrawLine(0, 0, 0, -100);

	// x���� ������
	IDREngine::GetInstance()->SetBrushColor(1.f, 0.f, 0.f, 1.f);
	IDREngine::GetInstance()->DrawLine(0, 0, 100, 0);

	IDREngine::GetInstance()->SetBrushColor(1.f, 1.f, 1.f, 1.f);
}

