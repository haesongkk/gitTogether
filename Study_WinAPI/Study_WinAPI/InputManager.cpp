#include "InputManager.h"

/*
	[���� ������ Ű enum �� ���� GetAsync �Լ�����
	����ϴ� Ű ���� �����ֱ� ���� �뵵]

	enum �� ���� ���� ������ �Է����ָ� 
	���� �Ȱ��� ���� �� �ְ� ��
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

	//LAST,		// enum �� ��
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
// ������ - ���� �ʱ�ȭ�� �����Ѵ�
{

}

InputManager::~InputManager()
// �Ҹ���
{

}

void InputManager::InitInput()
{
	// �ݺ����� ���� ó�� ����, ���� ���¸� ��� false�� �ʱ�ȭ
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		// push_back �� ������ ���� ��Ҹ� �߰�
		keyInfo[i].keyState = KEY_STATE::NONE;
		keyInfo[i].bPreDown = false;
	}

	// ���� ��� (����)
	// keyInfo[(int)EY::LEFT].eState;
}

void InputManager::UpdateInput()
{
	HWND hWnd = GetFocus();

	// �� nullptr �� �������°�..
	// ������ ��Ŀ�� ���� -> Ű �����ϰ�
	if (nullptr != hWnd)
	{
		// �ݺ����� ���� ��� Ű�� ���¸� üũ�Ѵ�
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// Ű�� ���� �����ΰ�?
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				// �̹� �����ִ� Ű�ΰ�?
				if (keyInfo[i].bPreDown)
				{
					keyInfo[i].keyState = KEY_STATE::HOLD;
				}
				// ó�� ���� ���ΰ�?
				else
				{
					keyInfo[i].keyState = KEY_STATE::TAP;
				}

				// �ƹ�ư �������Ƿ� ���°� ��
				keyInfo[i].bPreDown = true;
			}
			// Ű�� �ȴ����ִ� ����
			else
			{
				// �����ִ� ������ ���ΰ�?
				if (keyInfo[i].bPreDown)
				{
					keyInfo[i].keyState = KEY_STATE::AWAY;
				}
				// ��� �������ִ� ���ΰ�?
				else
				{
					keyInfo[i].keyState = KEY_STATE::NONE;
				}

				// �ƹ�ư �����Ƿ� ���°� ����
				keyInfo[i].bPreDown = false;
			}
		}
	}
	// ��Ŀ�� ���� ���� ���� -> Ű�� �� �� ���·� ����
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
