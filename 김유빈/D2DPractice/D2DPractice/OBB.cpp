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
	// 단위벡터화
	normal = normal.Normalize();
	// 내적 & min max 구하기
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

	// 단위벡터화
	normal = normal.Normalize();
	// 내적 & min max 구하기
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
	// 단위벡터화
	normal = normal.Normalize();
	// 내적 & min max 구하기
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
	// 단위벡터화
	normal = normal.Normalize();
	// 내적 & min max 구하기
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


// 우선 과정을 생각하면
// 
// 1. A B 물체 모두에 대한 각 변의 법선 벡터
// -> 근데 직사각형으로 고정하면 벡터니까 수직인 두 변만 확인해도 되나..
// 2. 법선 벡터에 대하여 도형을 정사영
// 3. 각 도형의 정사영은 min 과 max 로 체크하자
// -> 모든 법선벡터에 대하여
// -> 도형의 중심점 간의 거리 < 두 도형의 정사영 길이 이면 충돌
// -> 또는 min과 max를 기준으로 겹쳐도 충돌,, 이게 좋을듯

