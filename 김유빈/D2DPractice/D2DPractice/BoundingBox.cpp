//#include "CVECTOR2.h"
#include "BoundingBox.h"
#include <windows.h>
#include "InputManager.h"
#include <cmath>

#define PI 3.14159265 

BoundingBox::BoundingBox()
	:
	 m_moveSpeed(2.f)
	, isClicked(false)
	, degree(0.f)
	, temp(0.f)
	, radian(0.f)
{
}

BoundingBox::~BoundingBox()
{
}


void BoundingBox::Update()
{
	POINT mousePos = InputManager::GetInstance()->m_MousePos;
	if (InputManager::GetInstance()->GetKeyState(KEY::LBUTTON) == KEY_STATE::TAP)
	{
		if ((mousePos.x >= 20 && mousePos.x <= 480) && (mousePos.y >= 750 && mousePos.y <= 800))
		{
			isClicked = true;
		}
		else
		{
			isClicked = false;

		}
	}

	if (isClicked)
	{
		if (InputManager::GetInstance()->GetKeyState(KEY::Q) == KEY_STATE::HOLD)
		{
			Rotate(1);
		}
		else if (InputManager::GetInstance()->GetKeyState(KEY::E) == KEY_STATE::HOLD)
		{
			Rotate(-1);
		}
		else
		{
			if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53))
			{
				m_leftTop.y += m_moveSpeed;
				m_rightTop.y += m_moveSpeed;
				m_leftBottom.y += m_moveSpeed;
				m_rightBottom.y += m_moveSpeed;
			}
			if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57))
			{
				m_leftTop.y -= m_moveSpeed;
				m_rightTop.y -= m_moveSpeed;
				m_leftBottom.y -= m_moveSpeed;
				m_rightBottom.y -= m_moveSpeed;
			}
			if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41))
			{
				m_leftTop.x -= m_moveSpeed;
				m_rightTop.x -= m_moveSpeed;
				m_leftBottom.x -= m_moveSpeed;
				m_rightBottom.x -= m_moveSpeed;
			}
			if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44))
			{
				m_leftTop.x += m_moveSpeed;
				m_rightTop.x += m_moveSpeed;
				m_leftBottom.x += m_moveSpeed;
				m_rightBottom.x += m_moveSpeed;
			}

			/// CenterPos Calc 
			// 직각 사각형이라는 전제 하에 중심은 마주보는 좌표/2
			centerPos.x = (m_leftTop.x + m_rightBottom.x) / 2;
			centerPos.y = (m_leftTop.y + m_rightBottom.y) / 2;
		}
	}

}

void BoundingBox::Rotate(int dir)
{
	// Counter Clockwise Rotation
	if (dir == 1)
	{
		radian = 0.05f;
	}
	// Clockwise Rotation
	if (dir == -1)
	{
		radian = -0.05f;
	}

	// 1. 중심축을 우선 원점으로 옮겨보자
	// 이는 곧 중심점만큼 다른 좌표 성분에서 빼주면 되는거 아닌가
	m_leftTop.x = m_leftTop.x - centerPos.x;
	m_leftTop.y = m_leftTop.y - centerPos.y;
	
	m_rightTop.x = m_rightTop.x - centerPos.x;
	m_rightTop.y = m_rightTop.y - centerPos.y;
	
	m_leftBottom.x = m_leftBottom.x - centerPos.x;
	m_leftBottom.y = m_leftBottom.y - centerPos.y;
	
	m_rightBottom.x = m_rightBottom.x - centerPos.x;
	m_rightBottom.y = m_rightBottom.y - centerPos.y;

	// 2. 회전한다 행렬 풀면 (xcos - ysin , xsin + ycos)
	temp = m_leftTop.x;
	m_leftTop.x = m_leftTop.x * cos(radian) + m_leftTop.y * sin(radian);
	m_leftTop.y = temp * sin(radian)*(-1.f) + m_leftTop.y * cos(radian);

	temp = m_rightTop.x;
	m_rightTop.x = m_rightTop.x * cos(radian) + m_rightTop.y * sin(radian);
	m_rightTop.y = temp * sin(radian) * (-1.f) + m_rightTop.y * cos(radian);

	temp = m_leftBottom.x;
	m_leftBottom.x = m_leftBottom.x * cos(radian) + m_leftBottom.y * sin(radian);
	m_leftBottom.y = temp * sin(radian) * (-1.f) + m_leftBottom.y * cos(radian);
	
	temp = m_rightBottom.x;
	m_rightBottom.x = m_rightBottom.x * cos(radian) + m_rightBottom.y * sin(radian);
	m_rightBottom.y = temp * sin(radian) * (-1.f) + m_rightBottom.y * cos(radian);

	// 3. 기존 중심 좌표만큼 네개의 좌표를 이동시킨다
	m_leftTop.x = m_leftTop.x + centerPos.x;
	m_leftTop.y = m_leftTop.y + centerPos.y;

	m_rightTop.x = m_rightTop.x + centerPos.x;
	m_rightTop.y = m_rightTop.y + centerPos.y;

	m_leftBottom.x = m_leftBottom.x + centerPos.x;
	m_leftBottom.y = m_leftBottom.y + centerPos.y;

	m_rightBottom.x = m_rightBottom.x + centerPos.x;
	m_rightBottom.y = m_rightBottom.y + centerPos.y;

}

