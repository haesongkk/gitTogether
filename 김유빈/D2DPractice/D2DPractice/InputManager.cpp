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
	// 윈도우 포커싱 알아내기.
	//HWND hMainWnd = CCore::GetInst()->GetMainHwnd();	
	HWND hWnd = GetFocus();

	// 윈도우 포커싱 중일 때 키 이벤트 동작
	if (nullptr != hWnd)
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// 키가 눌려있다.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPush)
				{
					// 이전에도 눌려있었다.
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					// 이전에 눌려있지 않았다.
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true;
			}

			// 키가 안눌려있다.
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					// 이전에 눌려있었다.
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					// 이전에도 안눌려있었다.
					m_vecKey[i].eState = KEY_STATE::NONE;
				}

				m_vecKey[i].bPrevPush = false;
			}
		}
		
		// 마우스 위치 계산 <렌더 좌표 기준>
		//POINT ptPos = {};
		// 근데 이게 모니터 전체 화면 기준이라 윈도우 창 기준으로 다시 계산 해야함
		GetCursorPos(&ptPos);
		// 이걸 스크린 기준으로 바꿔주는 것
		ScreenToClient(m_hwnd, &ptPos);

		m_MousePos.x = (float)ptPos.x;
		m_MousePos.y = (float)ptPos.y;
	}

	// 윈도우 포커싱 해제상태
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