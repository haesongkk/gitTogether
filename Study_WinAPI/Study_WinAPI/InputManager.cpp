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


InputManager::InputManager()
	:hMainWnd(NULL)
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
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}

	// ���� ��� (����)
	// m_vecKey[(int)EY::LEFT].eState;
}

void InputManager::UpdateInput()
{
	


	// �ݺ����� ���� ��� Ű�� ���¸� üũ�Ѵ�
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		// Ű�� ���� �����ΰ�?
		if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
		{
			// �̹� �����ִ� Ű�ΰ�?
			if (m_vecKey[i].bPrevPush)
			{
				m_vecKey[i].eState = KEY_STATE::HOLD;			
			}
			// ó�� ���� ���ΰ�?
			else
			{
				m_vecKey[i].eState = KEY_STATE::TAP;
			}

			// �ƹ�ư �������Ƿ� ���°� ��
			m_vecKey[i].bPrevPush = true;
		}
		// Ű�� �ȴ����ִ� ����
		else
		{
			// �����ִ� ������ ���ΰ�?
			if (m_vecKey[i].bPrevPush)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			// ��� �������ִ� ���ΰ�?
			else
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}

			// �ƹ�ư �����Ƿ� ���°� ����
			m_vecKey[i].bPrevPush = false;
		}
	}
}
