#include "InputManager.h"
//#include <windows.h>


int g_arrVK[(int)::KEY::LAST] =
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',

	VK_MENU, // ALT
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN, // ENTER
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,

	//LAST,
};

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

void InputManager::Initialize()
{
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE,false });
	}
}

void InputManager::Update()
{
	// ������ ��Ŀ�� �˾Ƴ���.
	//HWND hMainWnd = CCore::GetInst()->GetMainHwnd();	
	HWND hWnd = GetFocus();

	// ������ ��Ŀ�� ���� �� Ű �̺�Ʈ ����
	if (nullptr != hWnd)
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// Ű�� �����ִ�.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPush)
				{
					// �������� �����־���.
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					// ������ �������� �ʾҴ�.
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true;
			}

			// Ű�� �ȴ����ִ�.
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					// ������ �����־���.
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					// �������� �ȴ����־���.
					m_vecKey[i].eState = KEY_STATE::NONE;
				}

				m_vecKey[i].bPrevPush = false;
			}
		}
		
		// ���콺 ��ġ ��� <���� ��ǥ ����>
		//POINT ptPos = {};
		// �ٵ� �̰� ����� ��ü ȭ�� �����̶� ������ â �������� �ٽ� ��� �ؾ���
		GetCursorPos(&ptPos);
		// �̰� ��ũ�� �������� �ٲ��ִ� ��
		ScreenToClient(m_hwnd, &ptPos);

		m_MousePos.x = (float)ptPos.x;
		m_MousePos.y = (float)ptPos.y;
	}

	// ������ ��Ŀ�� ��������
	else
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;

			if (KEY_STATE::TAP == m_vecKey[i].eState || KEY_STATE::HOLD == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}

	}
}