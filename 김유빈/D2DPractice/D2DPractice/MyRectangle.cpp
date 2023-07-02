#include "MyRectangle.h"
#include <windows.h>
#include "InputManager.h"
#include <cmath>

MyRectangle::MyRectangle()
	:m_Left(0)
	, m_Right(0)
	, m_Top(0)
	, m_Bottom(0)
	, m_moveSpeed(2.f)
	, isClicked(false)
{
}

MyRectangle::~MyRectangle()
{
}

void MyRectangle::Update()
{
	POINT mousePos = InputManager::GetInstance()->m_MousePos;

	if (InputManager::GetInstance()->GetKeyState(KEY::LBUTTON) == KEY_STATE::TAP)
	{
		if ((mousePos.x >= 20 && mousePos.x <= 480) && (mousePos.y >= 700 && mousePos.y <= 750))
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
			m_Top += m_moveSpeed;
			m_Bottom += m_moveSpeed;
		}
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57))
		{
			m_Top -= m_moveSpeed;
			m_Bottom -= m_moveSpeed;
		}
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41))
		{
			m_Left -= m_moveSpeed;
			m_Right -= m_moveSpeed;
		}
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44))
		{
			m_Left += m_moveSpeed;
			m_Right += m_moveSpeed;
		}
	}
}