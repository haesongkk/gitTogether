#include "AABB.h"
#include "MyRectangle.h"

AABB::AABB()
{
}

AABB::~AABB()
{
}

bool AABB::CollisionCheck()
{
	// �ʹ� ��ϱ� �켱 �¿� ���� üũ�Ѵ�
	if ((rect1->m_Left <= rect2->m_Right && rect1->m_Left >= rect2->m_Left) ||
		(rect1->m_Right <= rect2->m_Right && rect1->m_Right >= rect2->m_Left)||
		(rect1->m_Right >= rect2->m_Right && rect1->m_Left <= rect2->m_Left))
	{
		// �¿츦 üũ���� ���� �浹 ���� ���� ���Դٸ� ���Ʒ��� üũ�Ѵ�
		if ((rect1->m_Top >= rect2->m_Top && rect1->m_Top <= rect2->m_Bottom) ||
			(rect1->m_Bottom >= rect2->m_Top && rect1->m_Bottom <= rect2->m_Bottom)||
			(rect1->m_Bottom >= rect2->m_Bottom && rect1->m_Top <= rect2->m_Top))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	
}

void AABB::SetTarget(MyRectangle* _rect1, MyRectangle* _rect2)
{
	rect1 = _rect1;
	rect2 = _rect2;
}
