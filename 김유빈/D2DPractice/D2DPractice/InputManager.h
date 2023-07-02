#pragma once
#include <windows.h>

#include <vector>
using namespace std;

enum class KEY_STATE
{
	NONE, // ������ �ʾҰ�, �������� ������ ���� ����
	TAP, // �� ���� ����
	HOLD, // ������ �ִ� ����
	AWAY, // �� �� ����
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	LBUTTON,
	RBUTTON,

	LAST,
};

struct tKeyInfo
{
	KEY_STATE	eState; // Ű�� ���°�
	bool		bPrevPush;  // ���� �����ӿ��� ���ȴ��� ����
};

class InputManager
{
private:
	// ��Ŭ������ ����������
	InputManager();
	~InputManager();

	static InputManager* m_pInstance;

	vector<tKeyInfo> m_vecKey;
	HWND m_hwnd;
	POINT ptPos;

public:
	static InputManager* GetInstance()
	{
		/*if (m_pInstance == nullptr)
		{
			m_pInstance = new InputManager();
		}
		return m_pInstance;*/
		static InputManager mgr;
		return &mgr;
	}
	
	void Initialize();
	void Update();
	void SetMainHwnd(HWND _hwnd)
	{
		m_hwnd = _hwnd;
	};

	POINT m_MousePos;

public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }

};
