#include "Vector2.h"
#include "OBB.h"
#include "BoundingBox.h"
#include <algorithm>

using namespace std;

OBB::OBB()
{
}

OBB::~OBB()
{
}

bool OBB::CollisionCheck()
{
	bool hasCollision[4] = { false , false, false , false };
	
	float arr1[4] = { 0.f, };
	float arr2[4] = { 0.f, };
	float max_value1 = 0.f;
	float min_value1 = 0.f;
	float max_value2 = 0.f;
	float min_value2 = 0.f;

	/// rect 2
	normal = rect2->m_leftTop - rect2->m_leftBottom;
	// ��������ȭ
	normal = normal.Normalize();
	// ���� & min max ���ϱ�
	arr1[0] = rect1->m_leftTop.DotProduct(normal);
	arr1[1] = rect1->m_leftBottom.DotProduct(normal);
	arr1[2] = rect1->m_rightTop.DotProduct(normal);
	arr1[3] = rect1->m_rightBottom.DotProduct(normal);
	
	max_value1 = *max_element(arr1, arr1 + 4);
	min_value1 = *min_element(arr1, arr1 + 4);
	
	arr2[0] = rect2->m_leftTop.DotProduct(normal);
	arr2[1] = rect2->m_leftBottom.DotProduct(normal);
	arr2[2] = rect2->m_rightTop.DotProduct(normal);
	arr2[3] = rect2->m_rightBottom.DotProduct(normal);

	max_value2 = *max_element(arr2, arr2 + 4);
	min_value2 = *min_element(arr2, arr2 + 4);

	if ((min_value1 < max_value2 && min_value2 < max_value1))
	{
		hasCollision[0] = true;
	}

	/*if ((max_value1 <= max_value2 && max_value1 >= min_value2) ||
		(min_value1 <= max_value2 && min_value1 >= min_value2) ||
		(max_value1 >= max_value2 && min_value1 <= min_value2))
	{
		hasCollision[0] = true;
	}*/

	normal = rect2->m_rightTop - rect2->m_leftTop;

	// ��������ȭ
	normal = normal.Normalize();
	// ���� & min max ���ϱ�
	arr1[0] = rect1->m_leftTop.DotProduct(normal);
	arr1[1] = rect1->m_leftBottom.DotProduct(normal);
	arr1[2] = rect1->m_rightTop.DotProduct(normal);
	arr1[3] = rect1->m_rightBottom.DotProduct(normal);

	max_value1 = *max_element(arr1, arr1 + 4);
	min_value1 = *min_element(arr1, arr1 + 4);

	arr2[0] = rect2->m_leftTop.DotProduct(normal);
	arr2[1] = rect2->m_leftBottom.DotProduct(normal);
	arr2[2] = rect2->m_rightTop.DotProduct(normal);
	arr2[3] = rect2->m_rightBottom.DotProduct(normal);

	max_value2 = *max_element(arr2, arr2 + 4);
	min_value2 = *min_element(arr2, arr2 + 4);

	if ((min_value1 < max_value2 && min_value2 < max_value1))
	{
		hasCollision[1] = true;
	}

	/*if ((max_value1 <= max_value2 && max_value1 >= min_value2) ||
		(min_value1 <= max_value2 && min_value1 >= min_value2) ||
		(max_value1 >= max_value2 && min_value1 <= min_value2))
	{
		hasCollision[1] = true;
	}*/

	/// rect 1
	normal = rect1->m_rightTop - rect1->m_leftTop;
	// ��������ȭ
	normal = normal.Normalize();
	// ���� & min max ���ϱ�
	arr1[0] = rect1->m_leftTop.DotProduct(normal);
	arr1[1] = rect1->m_leftBottom.DotProduct(normal);
	arr1[2] = rect1->m_rightTop.DotProduct(normal);
	arr1[3] = rect1->m_rightBottom.DotProduct(normal);

	max_value1 = *max_element(arr1, arr1 + 4);
	min_value1 = *min_element(arr1, arr1 + 4);

	arr2[0] = rect2->m_leftTop.DotProduct(normal);
	arr2[1] = rect2->m_leftBottom.DotProduct(normal);
	arr2[2] = rect2->m_rightTop.DotProduct(normal);
	arr2[3] = rect2->m_rightBottom.DotProduct(normal);

	max_value2 = *max_element(arr2, arr2 + 4);
	min_value2 = *min_element(arr2, arr2 + 4);

	if ((min_value1 < max_value2 && min_value2 < max_value1))
	{
		hasCollision[2] = true;
	}

	/*if ((max_value1 <= max_value2 && max_value1 >= min_value2) ||
		(min_value1 <= max_value2 && min_value1 >= min_value2) ||
		(max_value1 >= max_value2 && min_value1 <= min_value2))
	{
		hasCollision[2] = true;
	}*/
	
	normal = rect1->m_leftTop - rect1->m_leftBottom;
	// ��������ȭ
	normal = normal.Normalize();
	// ���� & min max ���ϱ�
	arr1[0] = rect1->m_leftTop.DotProduct(normal);
	arr1[1] = rect1->m_leftBottom.DotProduct(normal);
	arr1[2] = rect1->m_rightTop.DotProduct(normal);
	arr1[3] = rect1->m_rightBottom.DotProduct(normal);

	max_value1 = *max_element(arr1, arr1 + 4);
	min_value1 = *min_element(arr1, arr1 + 4);

	arr2[0] = rect2->m_leftTop.DotProduct(normal);
	arr2[1] = rect2->m_leftBottom.DotProduct(normal);
	arr2[2] = rect2->m_rightTop.DotProduct(normal);
	arr2[3] = rect2->m_rightBottom.DotProduct(normal);

	max_value2 = *max_element(arr2, arr2 + 4);
	min_value2 = *min_element(arr2, arr2 + 4);

	if ((min_value1 < max_value2 && min_value2 < max_value1))
	{
		hasCollision[3] = true;
	}

	if (hasCollision[0] && hasCollision[1] && hasCollision[2] && hasCollision[3])
	{
		return true;
	}
	else
	{
		return false;
	}
}

void OBB::SetTarget(BoundingBox* _rect1, BoundingBox* _rect2)
{
	rect1 = _rect1;
	rect2 = _rect2;
}


// �켱 ������ �����ϸ�
// 
// 1. A B ��ü ��ο� ���� �� ���� ���� ����
// -> �ٵ� ���簢������ �����ϸ� ���ʹϱ� ������ �� ���� Ȯ���ص� �ǳ�..
// 2. ���� ���Ϳ� ���Ͽ� ������ ���翵
// 3. �� ������ ���翵�� min �� max �� üũ����
// -> ��� �������Ϳ� ���Ͽ�
// -> ������ �߽��� ���� �Ÿ� < �� ������ ���翵 ���� �̸� �浹
// -> �Ǵ� min�� max�� �������� ���ĵ� �浹,, �̰� ������

