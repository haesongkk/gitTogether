#include "Circle.h"
#include <windows.h>
#include "InputManager.h"
#include <cmath>

Circle::Circle()
	:m_posx(0),
	m_posy(0),
	m_radius(1),
	m_moveSpeed(2.f),
	isClicked(false)
{
}

Circle::~Circle()
{
}

void Circle::Update()
{
	POINT mousePos = InputManager::GetInstance()->m_MousePos;

	//int clickArea = pow(m_radius, 2);
	//int clickDistance = pow(m_posx - mousePos.x, 2) + pow(m_posy - mousePos.y, 2);

	/*if (clickArea >= clickDistance)
	{
		if (InputManager::GetInstance()->GetKeyState(KEY::LBUTTON) == KEY_STATE::TAP)
		{
			isClicked = true;
		}
	}
	if (clickArea < clickDistance)
	{
		if (InputManager::GetInstance()->GetKeyState(KEY::LBUTTON) == KEY_STATE::TAP)
		{
			isClicked = false;
		}
	}*/

	if (InputManager::GetInstance()->GetKeyState(KEY::LBUTTON) == KEY_STATE::TAP)
	{
		if ((mousePos.x >= 20 && mousePos.x <= 480) && (mousePos.y >= 650 && mousePos.y <= 700))
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
		if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53))
		{
			m_posy += m_moveSpeed;
		}
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57))
		{
			m_posy -= m_moveSpeed;
		}
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41))
		{
			m_posx -= m_moveSpeed;
		}
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44))
		{
			m_posx += m_moveSpeed;
		}
	}

}