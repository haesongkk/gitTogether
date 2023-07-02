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
	// 너무 기니까 우선 좌우 먼저 체크한다
	if ((rect1->m_Left <= rect2->m_Right && rect1->m_Left >= rect2->m_Left) ||
		(rect1->m_Right <= rect2->m_Right && rect1->m_Right >= rect2->m_Left)||
		(rect1->m_Right >= rect2->m_Right && rect1->m_Left <= rect2->m_Left))
	{
		// 좌우를 체크했을 때에 충돌 범위 내에 들어왔다면 위아래를 체크한다
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
