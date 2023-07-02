#include "CC.h"
#include "Circle.h"
#include <cmath>

CC::CC()
{
}

CC::~CC()
{
}

bool CC::CollisionCheck()
{
	// ��ġ�� �޾ƿ���
	x_c1 = c1->m_posx;
	y_c1 = c1->m_posy;
	rad_c1 = c1->m_radius;

	x_c2 = c2->m_posx;
	y_c2 = c2->m_posy;
	rad_c2 = c2->m_radius;

	float distance; // �������� �Ÿ�
	float boundary = (float)rad_c1 + (float)rad_c2; // �浹 ��谪
	distance = sqrt(pow((x_c1 - x_c2), 2) + pow((y_c1 - y_c2), 2));

	if (distance > boundary)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CC::SetTarget(Circle* _c1, Circle* _c2)
{
	c1 = _c1;
	c2 = _c2;
}


