#include "InputManager.h"

/*
	[내가 설정한 키 enum 과 실제 GetAsync 함수에서
	사용하는 키 값을 맞춰주기 위한 용도]

	enum 에 따라 같은 순서로 입력해주면 
	값을 똑같이 맞출 수 있게 됨
*/
int g_arrVK[(int)KEY::LAST] =
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
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',
	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,

	//LAST,		// enum 의 끝
};

InputManager* InputManager::instance = nullptr;

InputManager* InputManager::GetInst() {
	//static InputManager instance = nullptr;
	//return &instance;

	if (instance == nullptr)
	{
		instance = new InputManager();
	}
	return instance;

}

InputManager::InputManager()
// 생성자 - 변수 초기화를 진행한다
{

}

InputManager::~InputManager()
// 소멸자
{

}

void InputManager::InitInput()
{
	// 반복문을 돌며 처음 상태, 이전 상태를 모두 false로 초기화
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		// push_back 은 벡터의 끝에 요소를 추가
		keyInfo[i].keyState = KEY_STATE::NONE;
		keyInfo[i].bPreDown = false;
	}

	// 접근 방식 (예시)
	// keyInfo[(int)EY::LEFT].eState;
}

void InputManager::UpdateInput()
{
	HWND hWnd = GetFocus();

	// 왜 nullptr 이 먼저오는가..
	// 윈도우 포커싱 상태 -> 키 동작하게
	if (nullptr != hWnd)
	{
		// 반복문을 돌며 모든 키의 상태를 체크한다
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// 키가 눌린 상태인가?
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				// 이미 눌려있던 키인가?
				if (keyInfo[i].bPreDown)
				{
					keyInfo[i].keyState = KEY_STATE::HOLD;
				}
				// 처음 눌린 것인가?
				else
				{
					keyInfo[i].keyState = KEY_STATE::TAP;
				}

				// 아무튼 눌렸으므로 상태값 참
				keyInfo[i].bPreDown = true;
			}
			// 키가 안눌려있는 상태
			else
			{
				// 눌려있다 떨어진 것인가?
				if (keyInfo[i].bPreDown)
				{
					keyInfo[i].keyState = KEY_STATE::AWAY;
				}
				// 계속 떨어져있던 것인가?
				else
				{
					keyInfo[i].keyState = KEY_STATE::NONE;
				}

				// 아무튼 뗐으므로 상태값 거짓
				keyInfo[i].bPreDown = false;
			}
		}
	}
	// 포커싱 되지 않은 상태 -> 키를 다 뗌 상태로 돌림
	else
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			keyInfo[i].bPreDown = false;

			if (KEY_STATE::TAP == keyInfo[i].keyState || KEY_STATE::HOLD == keyInfo[i].keyState)
			{
				keyInfo[i].keyState = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == keyInfo[i].keyState)
			{
				keyInfo[i].keyState = KEY_STATE::NONE;
			}
		}
	}
}
