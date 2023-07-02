#pragma once
#include <windows.h>

#include <vector>
using namespace std;

enum class KEY_STATE
{
	NONE, // 눌리지 않았고, 이전에도 눌리지 않은 상태
	TAP, // 막 누른 시점
	HOLD, // 누르고 있는 시점
	AWAY, // 막 뗀 시점
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
	KEY_STATE	eState; // 키의 상태값
	bool		bPrevPush;  // 이전 프레임에서 눌렸는지 여부
};

class InputManager
{
private:
	// 싱클턴으로 만들어버리기
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
